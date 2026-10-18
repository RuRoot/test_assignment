#include <test_rendering.h>
#include <test_manager.h>
#include <test_gr_draw.h>
#include <test.h>
#include <mesh.h>
#include <last.h>

using namespace c3d;


//------------------------------------------------------------------------------
// Раскрасить объекты в отдельном окне
// ---
void ScanLine( RPArray<MbItem> & objs )
{
//  clock_t st = clock();                        // Засекаем время
  size_t objsCount = objs.Count();
  RPArray<MbGrid> gridirons(objsCount,5);
  SArray<double> ambient(objsCount,5);
  SArray<double> diffuse(objsCount,5);
  SArray<double> specularity(objsCount,5);
  SArray<double> shininess(objsCount,5);
  SArray<double> opacity(objsCount,5);
  SArray<uint32> solors(objsCount,5);
  float am = (float)0.4;  // Общий фон
  float df = (float)0.3;  // Диффузия
  float sp = (float)0.3;  // Зеркальность
  float sh = (float)25;   // Блеск
  float tr = (float)1.0;  // Непрозрачность
  float em = (float)0.0;  // Излучение
  uint32 cl;
  double sag = Math::visualSag; // / vc->scale;
  ptrdiff_t i;
  MbRegDuplicate * iReg = nullptr;
  MbAutoRegDuplicate autoReg( iReg );  

  // Триангуляция объектов
  for ( i = 0; i < (ptrdiff_t)objs.Count(); i++ ) {
    //size_t gridironsCount = gridirons.Count();
    MbItem * meshItem = objs[i]->CreateMesh( MbStepData(ist_SpaceStep, sag), MbFormNote(false, true, true, TestVariables::exactMesh), iReg );
    cl = objs[i]->GetColor();
    objs[i]->GetVisual( am, df, sp, sh, tr, em );
    if ( (meshItem != nullptr) && (meshItem->IsA() == st_Mesh) ) {
      MbMesh * mesh = (MbMesh *)meshItem;
      for ( ptrdiff_t j = mesh->GridsCount(); j >= 0; j-- ) {
        MbGrid * gr = mesh->DetachGrid( j );
        if ( gr != nullptr ) {
          gridirons.Add( gr );
          ambient.Add( am );
          diffuse.Add( df );
          specularity.Add( sp );
          shininess.Add( sh );
          opacity.Add( tr );
          solors.Add( cl );
        }
      }
    }
    ::DeleteItem( meshItem );
    
    if ( PeekEscape() )
        return; // Если нажата клавиша, прекратить отрисовку
  }

  if ( gridirons.Count()>0 ) {
    if ( Math::tempIndex == 2 )
      ChangeNormals( gridirons ); // Изменить направление нормалей сетки случайным образом
    ScanLineRender( gridirons, ambient, diffuse, specularity, shininess, opacity, solors );
  }

  for ( size_t j = 0, jCount = gridirons.Count(); j < jCount; j++ ) 
    delete gridirons[j];
}


//------------------------------------------------------------------------------
// Раскрасить объекты в отдельном окне
// ---
void ScanLineRender( RPArray<MbGrid> & gridirons,
                     SArray<double> & ambient, SArray<double> & diffuse,
                     SArray<double> & specularity, SArray<double> & shininess,
                     SArray<double> & opacity, SArray<uint32> & colors )
{
  MbGrid * gr = nullptr;
  ptrdiff_t i, j, k, l;
  WorkWindow * vc = TestVariables::viewManager->GetActiveWindow();
  IGrDraw & tool = vc->GetDrawTool();

  MbMatrix3D into( vc->GetScaleMatrixInto() );
  MbMatrix3D from( vc->GetScaleMatrixFrom() );
  MbRect globalGab;
  MbCartPoint3D pnt;
  ptrdiff_t tcount = 0; // Количество треугольников
  MbCartPoint3D vista(vc->GetVista());
  double xFactor = tool.GetXPixelPerMM();
  double yFactor = tool.GetYPixelPerMM();
  double zFactor = ONE_HALF * ( xFactor + yFactor );
  MbCube cube;
  double zmin, zmax, dz, kz = 0.95; // Затемнение при удаленности
  double ka, kd, ks, kb, kt;
  //MbVector3D norm, light0( tool.GetLight()/*lightDirection*/ );
  MbVector3D norm, light0 = tool.GetLight();

  // Установка источника света
  tool.GetGLProperty( ka, kd, ks, light0 );
  light0.Normalize();
  vista.x /= xFactor;
  vista.y /= yFactor;
  vista.z /= zFactor;
  vista.y = -vista.y;
  // Перевод четырёхугольников в треугольники
  for ( j = 0; j < (ptrdiff_t)gridirons.Count(); j++ ) {
    gridirons[j]->ConvertAllToTriangles();  // Перевод сетки
  }
  // Перевод точек и нормалей треугольников в экранную систему координат
  for ( j = 0; j < (ptrdiff_t)gridirons.Count(); j++ ) {
    gridirons[j]->Transform( into );  // Перевод сетки
    tcount += gridirons[j]->TrianglesCount();
  }
  if ( vista.z > DELTA_MIN) {
    for ( j = 0; j < (ptrdiff_t)gridirons.Count(); j++ )
      gridirons[j]->SetVista( vista );
  }

  // Определение габарита изображения
  for ( j = 0; j < (ptrdiff_t)gridirons.Count(); j++ ) {
    gridirons[j]->AddRect( globalGab );
    gridirons[j]->AddCube( cube );
  }

  if ( cube.IsEmpty() ) {
    zmin = 0;
    zmax = 1;
    dz = 1;
  }
  else {
    zmin = cube.pmin.z;
    zmax = cube.pmax.z;
    dz = fabs(zmax - zmin);
  }

  // Усечение габарита изображения размерами экрана
  RECT r;
  tool.GetClientRect( r );

  MbRect viewRect(double(r.left-1)  / xFactor, double(-r.bottom-1)  / yFactor,
                  double(r.right+1) / xFactor, double(-r.top+1)     / yFactor);

  if (globalGab.left   < viewRect.left)   globalGab.left   = viewRect.left;
  if (globalGab.bottom < viewRect.bottom) globalGab.bottom = viewRect.bottom;
  if (globalGab.right  > viewRect.right)  globalGab.right  = viewRect.right;
  if (globalGab.top    > viewRect.top)    globalGab.top    = viewRect.top;

  POINT pix  = { (int32)( globalGab.left  * xFactor ), (int32)( -globalGab.bottom * yFactor ) };
  POINT pix1 = { (int32)( globalGab.right * xFactor ), (int32)( -globalGab.top    * yFactor ) };
  
  pix.x = pix.x-1;
  pix.y = pix.y+1;
  pix1.x = pix1.x+1;
  pix1.y = pix1.y-1;

  // Разбиение изображения на клетки и сортировка треугольников по клеткам
  ptrdiff_t icount = (ptrdiff_t)(pix1.x - pix.x);
  ptrdiff_t jcount = (ptrdiff_t)(pix.y  - pix1.y);
  double gabDx = globalGab.right - globalGab.left;
  double gabDy = globalGab.top   - globalGab.bottom;
  double X_DISCRETE = gabDx / (double)icount;
  double Y_DISCRETE = gabDy / (double)jcount;
  double factor = std_min( 400.0, (double)tcount / 8.0 ); // Приблизительное число клеток разбиения изображения
  
  PRECONDITION( factor * gabDx * gabDy >= 0.0 );
  
  ptrdiff_t lineXCount = (ptrdiff_t)( ::sqrt( factor * gabDx / gabDy ) + 1 ); // Число полос разбиения по x
  ptrdiff_t lineYCount = (ptrdiff_t)( ::sqrt( factor * gabDy / gabDx ) + 1 ); // Число полос разбиения по y
  ptrdiff_t discreteXCount = icount / lineXCount + 1; // Число дискрет x в клетке
  ptrdiff_t discreteYCount = jcount / lineYCount + 1; // Число дискрет y в клетке
  PArray <SArray <ptrdiff_t> > boxes(lineXCount*lineYCount,2,true); // Множество индексов треугольников для клеток

  SetGridBoxes( gridirons, tcount, globalGab, X_DISCRETE, Y_DISCRETE, discreteXCount, discreteYCount,
                factor, lineXCount, lineYCount, boxes );

  // Отрисовка объектов
  ptrdiff_t nBoxCount;
  uint8 cR, cG, cB;
  POINT pp;
  int startx = pix.x;
  double delta = DELTA_MIN/vc->GetScale();
  double pointz;
  double ztransp0;
  double dColorR, dColorG, dColorB;

  double dR = 0.0;
  double dG = 0.0;
  double dB = 0.0;

  uint32 cl;
  SArray<double> ambs(5,5);
  SArray<double> difs(5,5);
  SArray<double> specs(5,5);
  SArray<double> shins(5,5);
  SArray<double> trans(5,5);
  SArray<double> cols(5,5);
  SArray<double> zbuffer(5,5);
  SArray<MbVector3D> normals(5,5);
  COLORREF bkColor;

  bkColor = vc->GetDrawTool().GetBackground();

  pnt.y = globalGab.top;
  pix.y = pix1.y;
  pix1 = pix;
  for ( j = 0; j < jcount; j++ ) {
    pnt.x = globalGab.left;
    pix.x = startx;
    ptrdiff_t/*OV_x64 int */ jLine = j / discreteYCount; // Номер полосы по y
    for ( i = 0; i < icount; i++ ) {
      pnt.z = -MB_MAXDOUBLE;
      pointz = -MB_MAXDOUBLE;
      ptrdiff_t index = -1;
      ambs.Flush();
      difs.Flush();
      specs.Flush();
      shins.Flush();
      trans.Flush();
      cols.Flush();
      zbuffer.Flush();
      normals.Flush();
      ptrdiff_t iLine = i / discreteXCount; // Номер полосы по x
      ptrdiff_t nLine = jLine * lineXCount + iLine;
      ptrdiff_t nBox;
      l = 0;
      gr = gridirons[l];         // Множество треугольников
      ka = ambient[l];
      kd = diffuse[l];
      ks = specularity[l];
      kb = shininess[l];
      kt = opacity[l];
      cl = colors[l]; // Цвет треугольника
      nBoxCount = boxes[nLine]->Count();
      for ( nBox = 0; nBox < nBoxCount; nBox++ ) {
        k = (*boxes[nLine])[nBox]; // Номер треугольника
        if ( k < 0 ) {
          gr = gridirons[l];       // Следующий объект
          ka = ambient[l];
          kd = diffuse[l];
          ks = specularity[l];
          kb = shininess[l];
          kt = opacity[l];
          cl = colors[l]; // Цвет треугольника
          l++;
        }
        else {
          if ( GetVisibleNormal( gr, k, kt < 1-DELTA_MIN, delta, pointz, pnt, norm ) ) {
            index = k;      // Признак существования объекта в точке
            ptrdiff_t i3 = 0;
            while ( (i3 < ((ptrdiff_t)zbuffer.Count())) && pointz < zbuffer[i3] )
              i3++;

            ambs.InsertInd(i3,ka);
            difs.InsertInd(i3,kd);
            specs.InsertInd(i3,ks);
            shins.InsertInd(i3,kb);
            trans.InsertInd(i3,kt);
            cols.InsertInd(i3,cl);
            zbuffer.InsertInd(i3,pointz);
//            SetNormalVector( norm, 10 );
            normals.InsertInd(i3,norm);
          }
        }
      }

      // Определение яркости точки объекта и отрисовка его точки
      if ( index != -1 ) {
        ztransp0 = 1;
        dColorR = 0;
        dColorG = 0;
        dColorB = 0;
        bool shade = false; // Флаг построения теней
        if ( Math::tempIndex == 1 )
          shade = InShadow( gridirons, pnt, light0 ); // Определение затенённости точки

        for ( ptrdiff_t i3 = 0; i3 < (ptrdiff_t)zbuffer.Count(); i3++ ) {
          if ( trans[i3] > LENGTH_EPSILON ) {
            norm = normals[i3];
            GetColors( light0, ambs[i3], difs[i3], specs[i3], shins[i3], shade,
                       zmin, zmax, kz, dz, zbuffer[i3], norm, (uint32)cols[i3], dR, dG, dB );
            dColorR += dR*ztransp0*trans[i3];
            dColorG += dG*ztransp0*trans[i3];
            dColorB += dB*ztransp0*trans[i3];
          }
          ztransp0 *= 1-trans[i3];
        }
        if ( ztransp0 > LENGTH_EPSILON ) {
          dColorR += (double)GetRValue(bkColor)*ztransp0;
          dColorG += (double)GetGValue(bkColor)*ztransp0;
          dColorB += (double)GetBValue(bkColor)*ztransp0;
        }
        cR = uint8( dColorR+0.5 );
        cG = uint8( dColorG+0.5 );
        cB = uint8( dColorB+0.5 );
        pp.x = pix.x; pp.y = pix.y;
        tool.SetPixel( pp.x, pp.y, cR, cG, cB );
      }
      pnt.x += X_DISCRETE;
      pix.x += 1;
    }
    pnt.y -= Y_DISCRETE;
    pix.y += 1;

    if ( PeekEscape() )
      break; // Если нажата клавиша, прекратить отрисовку
    else
    {
      TCHAR s[256];
      _sntprintf( s, 256, IDS_LINE_RENDER_DONE, double(j+1) / double(jcount) * 100.0 );  //-V111
      SetStatusBarText( s );  // Отрисовать текст
    }
  }

  // Перевод точек и нормалей триангуляции в исходное состояние
  if ( vista.z > DELTA_MIN ) {
    for ( j = 0; j < (ptrdiff_t)gridirons.Count(); j++ )
      gridirons[j]->DeleteVista( vista );
  }
  for ( i = 0; i < (ptrdiff_t)gridirons.Count(); i++ )
    gridirons[i]->Transform( from );  // Перевод сетки

#ifdef __USE_QT__
  TestVariables::viewManager->InvalidateWindow( TestVariables::viewManager->GetActiveWindowIndex(), true );
#endif // __USE_QT__
}


//------------------------------------------------------------------------------
// Разбиение изображения на клетки и сортировка треугольников по клеткам
// ---
void SetGridBoxes( RPArray<MbGrid> & gridirons, ptrdiff_t tcount, MbRect & globalGab,
                   double X_DISCRETE, double Y_DISCRETE, ptrdiff_t discreteXCount, ptrdiff_t discreteYCount,
                   double factor, ptrdiff_t lineXCount, ptrdiff_t lineYCount,
                   PArray< SArray<ptrdiff_t> > & boxes )
{
  SArray<ptrdiff_t> * box = nullptr;   // Номера треугольников входящих в клетку
  PArray<MbRect> localArr(lineXCount*lineXCount,2,true);
  MbRect * localGab = nullptr;
  MbRect  triGab;
  double dx = X_DISCRETE * (double)discreteXCount;
  double dy = Y_DISCRETE * (double)discreteYCount;
  double posx, posy;
  MbGrid * gr = nullptr;
  ptrdiff_t l = (ptrdiff_t)( (double)tcount/(factor+1) );

  posy = globalGab.top;
  for ( ptrdiff_t j = 0; j < lineYCount; j++ ) { // Строим габарит клетки по y
    posx = globalGab.left;
    for ( ptrdiff_t i = 0; i < lineXCount; i++ ) { // Строим габарит клетки по x
      localGab = new MbRect( posx, posy-dy, posx+dx, posy );
      localArr.Add(localGab);
      box = new SArray <ptrdiff_t> (l,50);
      boxes.Add( box );
      posx += dx;
    }
    posy -= dy;
  }

  for ( l = 0; l < (ptrdiff_t)gridirons.Count(); l++ ) {
    for ( ptrdiff_t i = 0; i < (ptrdiff_t)boxes.Count(); i++ )
      boxes[i]->Add(-10);
    gr = gridirons[l];
    for ( ptrdiff_t k = 0; k < (ptrdiff_t)gr->TrianglesCount(); k++ ) {
      gr->TriangleGetGabRect( k, triGab );
      for ( ptrdiff_t i = 0; i < (ptrdiff_t)boxes.Count(); i++ ) {
        if ( localArr[i]->left   > triGab.right )
          continue;
        if ( localArr[i]->right  < triGab.left )
          continue;
        if ( localArr[i]->bottom > triGab.top )
          continue;
        if ( localArr[i]->top    < triGab.bottom )
          continue;
        boxes[i]->Add(k);
      }
    }
  }
}


//------------------------------------------------------------------------------
// Находится ли точка pnt в тени gridirons
// ---
bool GetVisibleNormal( MbGrid * gr, ptrdiff_t k, bool transpBuffer, double delta,
                       double & pointz, MbCartPoint3D & pnt, MbVector3D & norm )
{
  MbFloatPoint3D p0, p1, p2;
  if ( gr->GetTrianglePoints( k, p0, p1, p2 ) ) {
    double x0 = p0.x;
    double x1 = p1.x;
    double x2 = p2.x;
    double y0 = p0.y;
    double y1 = p1.y;
    double y2 = p2.y;
    double d = (x1-x0)*(y2-y0) - (y1-y0)*(x2-x0); // Удвоенная площадь треугольника
    if ( fabs( d ) == 0 ) return false;
    d = 1.0 / d;
    // Проверка на попадание в треугольник
    double s0 = ( (x2-x1)*(pnt.y-y1) - (y2-y1)*(pnt.x-x1) ) * d;
    if (s0<-delta*fabs(d)) return false;
    double s1 = ( (x0-x2)*(pnt.y-y2) - (y0-y2)*(pnt.x-x2) ) * d;
    if (s1<-delta*fabs(d)) return false;
    double s2 = ( (x1-x0)*(pnt.y-y0) - (y1-y0)*(pnt.x-x0) ) * d;
    if (s2<-delta*fabs(d)) return false;
    // Проверка на близость к наблюдателю по сравнению с предыдущим треугольником
    double z0  = p0.z;
    double dz1 = p1.z - z0;
    double dz2 = p2.z - z0;
    z0 = z0 - ( (pnt.x-x0)*( (y1-y0)*dz2-dz1*(y2-y0) ) -
                (pnt.y-y0)*( (x1-x0)*dz2-dz1*(x2-x0) ) ) * d;
    if ( !transpBuffer && z0 <= pointz ) return false;
    if ( z0 > pnt.z )
      pnt.z = z0;
    pointz = z0;     // Близость к наблюдателю
    x0 = s0*d;
    y0 = s1*d;
    z0 = s2*d;
    MbFloatVector3D n0, n1, n2;
    bool planar = gr->GetSingleNormal ( n0 );
    if ( planar ) {
      n1 = n0;
      n2 = n0;
    }
    if ( planar || gr->GetTriangleNormals( k, n0, n1, n2 ) ) {
      norm.x = n0.x*x0 + n1.x*y0 + n2.x*z0; // Усредненная нормаль к поверхности
      norm.y = n0.y*x0 + n1.y*y0 + n2.y*z0; // Усредненная нормаль к поверхности
      norm.z = n0.z*x0 + n1.z*y0 + n2.z*z0; // Усредненная нормаль к поверхности
      return true;
    }
  }
  return false;
}


//------------------------------------------------------------------------------
// Находится ли точка pnt в тени gridirons
// ---
bool InShadow( RPArray<MbGrid> & gridirons, MbCartPoint3D & pnt, MbVector3D & light0 )
{
  MbGrid * gr = nullptr;
  MbCartPoint3D p, p0, p1, p2, p3;
  MbVector3D axis(light0);
  MbVector3D vect0, vect1, vect2, vect3;
  double l0, l1, l2, d, t1, t2, a1, a2;

//  axis.Invert();
  axis.Normalize();
  for ( ptrdiff_t j=0; j<(ptrdiff_t)gridirons.Count(); j++ ) {
    gr = gridirons[j];
    for ( ptrdiff_t k=0; k<(ptrdiff_t)gr->TrianglesCount(); k++ ) {
      if ( gr->GetTrianglePoints( k, p0, p1, p2 ) ) {
        vect0.Init(p0,pnt);
        vect1.Init(p0,p1);
        vect2.Init(p0,p2);
        vect0 = vect0 - axis*(vect0*axis);
        vect1 = vect1 - axis*(vect1*axis);
        vect2 = vect2 - axis*(vect2*axis);
        l1 = vect1.Length();
        l2 = vect2.Length();
        if (l1==0 || l2==0)
          continue;
        vect1 /= l1;
        vect2 /= l2;
        l0 = vect1*vect2;
        d = 1.0 - l0*l0;
        if (d==0)
          continue;
        a1 = vect0*vect1;
        a2 = vect0*vect2;
        t1 = (a1 - a2*l0)/d/l1;
        t2 = (a2 - a1*l0)/d/l2;
        if ( t1<0 || t2<0 || t1+t2>1 )
          continue;
        vect1.Init(p0,p1);
        vect2.Init(p0,p2);
        vect1 *= t1;
        vect2 *= t2;
        p = p0 + vect1 + vect2;
        vect0.Init(p,pnt);
        l0 = vect0.Length();
        if (l0>DELTA_MIN) {
          vect0 /= l0;
          d = vect0*axis;
          if (d<-DELTA_MIN)
            return true;
        }
      }
    }
  }
  return false;
}


//------------------------------------------------------------------------------
// Определентие цвета
// ---
void GetColors( MbVector3D & light0, double ka, double kd, double ks, double kb, bool shade,
                double zmin, double zmax, double kz, double dz, double pointz, MbVector3D & norm,
                uint32 color, double & dColorR, double & dColorG, double & dColorB )
{
  norm.Normalize();
  PRECONDITION( fabs(light0.z) <= 1.0 );
  double tet1 = fabs( cos( 0.5 * _acos( fabs( light0.z ) ) ) );
  double kn = (ka + kd*tet1 + ks) + DOUBLE_EPSILON; // Нормирующий коэффициент
  ka /= kn*0.5;
  kd /= kn*0.5;
  ks /= kn*0.5;
  double koef, cosa = light0*norm;
  double kr = ((pointz-zmin) + kz*(zmax-pointz))/dz;
  if ( shade || cosa<0 ) { // Точка находится в тени
    kd /= 2;
    koef = kr*ka + kd*cosa;
  }
  else {
    double cosb = cosa*fabs(norm.z) - 0.5*light0.z + 0.5;
    double b = pow( cosb, kb );
    koef = kr*ka + kd*cosa + ks*b;
  }
  double colorR = GetRValue(color);
  double colorG = GetGValue(color);
  double colorB = GetBValue(color);
  if ( koef < 1 ) {
    dColorR = koef * colorR;
    dColorG = koef * colorG;
    dColorB = koef * colorB;
  }
  else {
    double z0 = ( koef - 1.0 ) * 255.0;
    double kf = 2.0 - koef;
    dColorR = z0 + kf * colorR;
    dColorG = z0 + kf * colorG;
    dColorB = z0 + kf * colorB;
  }
}


//------------------------------------------------------------------------------
// Изменить направление нормалей сетки случайным образом
// ---
void ChangeNormals( RPArray<MbGrid> & gridirons )
{
  MbVector3D norm;
  ptrdiff_t i, j;

  for ( j=0; j<(ptrdiff_t)gridirons.Count(); j++ ) {
    for ( i=0; i<(ptrdiff_t)gridirons[j]->NormalsCount(); i++ ) {
      gridirons[j]->GetNormal(i,norm);
      SetNormalVector( norm, 30 );
      gridirons[j]->SetNormal(i,norm);
    }
  }
}


//------------------------------------------------------------------------------
// Изменить направление нормали случайным образом
// ---
void SetNormalVector( MbVector3D & norm, int angle0 )
{
  MbVector3D v0[3];
  MbVector3D vx;
  MbAxis3D axis;
  double d, a0[3];
  int m0 = rand();//0;
  int azim, angle;
  int azim0 = 360;
  ptrdiff_t i, i0;

  v0[0].Init(1,0,0);
  v0[1].Init(0,1,0);
  v0[2].Init(0,0,1);
  for (i=0;i<3;i++)
    a0[i] = norm*v0[i];
  i0 = 0;
  d = a0[i0];
  for (i=1;i<3;i++)
    if (::fabs(a0[i])<d) {
      i0 = i;
      d = a0[i0];
    }
  
  vx = v0[i0] - norm*(d);
  vx.Normalize();

  azim = m0%azim0;
  angle = m0%angle0;

  axis.SetAxisZ() = norm;
  vx.Rotate( axis, M_PI/180*azim );
  axis.SetAxisZ() = vx;
  norm.Rotate( axis, M_PI/180*angle );
}




