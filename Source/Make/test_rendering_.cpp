#include <test_rendering.h>
#include <test_rendering_context.h>
#include <test_gr_draw.h>
#include <test.h>
#include <test_manager.h>
#include <test_variables.h>
#include <curve.h>
#include <last.h>

using namespace c3d;


//------------------------------------------------------------------------------
// Поднятие флага возможности печати
// ---
void UpPlotFlag() {
  TestVariables::plotFlag = 1;
}


//------------------------------------------------------------------------------
// 
// ---
static bool GetStyle( COLORREF init, COLORREF &color, double &width ) {
  double w = width;
  int cr, cb, cg, vr=0, vb=0, vg=0;

  width = 0.8;
  cr = GetRValue( init );
  cg = GetGValue( init );
  cb = GetBValue( init );
  if (cr>250 && cg>250 && cb>250) {
    vr = 250;
    vg = 250;
    vb = 250;
    width = 0.1;
  }
  else
  if (cr>126) {
    vr = 255;
    width = 0.2;
  }  
  else
  if (cg>126) {
    vg = 150;
    width = 0.4;
  }  
  else
  if (cb>126) {
    vb = 255;
    width = 0.6;
  }  

  color = RGB(vr,vg,vb); 
  
  return fabs(w-width)<PARAM_NEAR;
}
        

//------------------------------------------------------------------------------
// Выбор имени файла
// ---
TCHAR * FileName() {
  if ( _GetSaveFileName( TestVariables::nameBuffer, WRITE_FILE_INFO, BMP_EXT_INFO, BMP_EXT ) )
  {
    static TCHAR szFile[STRINGLENGTH];
    _tcscpy(szFile, TestVariables::nameBuffer);
    return szFile;
  }
  else
    return nullptr;
}


//------------------------------------------------------------------------------
// Подготовка
// ---
bool IsFullVisibleInRect( const MbCurve * curve, const MbRect & rect )
{
  if ( curve != nullptr ) {
    MbCartPoint p;
    double t, tmax;
    ptrdiff_t i0;

    tmax = curve->GetTMax();
    t = curve->GetTMin();
    curve->PointOn( t, p );
    if ( !rect.Contains( p ) )
      return false;

    i0 = 1;
    while ( i0 > 0 ) {
      t += curve->Step( t, Math::visualSag );
      if (t>tmax-PARAM_REGION) {
        t = tmax;
        i0 = 0;
      }
      curve->PointOn( t, p );
      if ( !rect.Contains( p ) )
        return false;
    }

    return true;
  }

  return false;
}


//------------------------------------------------------------------------------
// Отрисовка
// ---
void DrawPlot( WorkWindow *wnd, POINT cursorPoint0, POINT cursorPoint1, PArray <MbCurve> &wire, SArray <Style> &styles ) {
  double xFactor = wnd->GetDrawTool().GetXPixelPerMM(); //double( ::GetDeviceCaps(dc,LOGPIXELSX) )/25.4; // pixel_per_mm
  double yFactor = wnd->GetDrawTool().GetYPixelPerMM(); //double( ::GetDeviceCaps(dc,LOGPIXELSX) )/25.4; // pixel_per_mm

  POINT pp;
  MbCartPoint p;
  double t, tmax;
  double width = 0.8;
  COLORREF color = RGB(0,0,0);
  ptrdiff_t i, i0;

  int x0 = std_min( cursorPoint0.x, cursorPoint1.x );
  int y0 = std_min( cursorPoint0.y, cursorPoint1.y );
  ptrdiff_t ix = (ptrdiff_t)::abs_t(cursorPoint0.x - cursorPoint1.x) + 1;
  ptrdiff_t iy = (ptrdiff_t)::abs_t(cursorPoint0.y - cursorPoint1.y) + 1;

  IGrDraw& tool = wnd->GetDrawTool();

//  COLORREF col; //GetSysColor( COLOR_WINDOW );
//  wnd->GetBackground( col );

  RECT rc;
    if (x0>0) x0--;
    if (y0>0) x0--;
    rc.left    = x0;
    rc.top     = y0;
    rc.right   = x0 + (int)LoUint32(ix);
    rc.bottom  = y0 + (int)LoUint32(iy);
  tool.FillRectangle( &rc ); // Залить прямоугольник цветом фона


  tool.SelectPen( 1/*width*/, 0, 0, 0 );

  for ( i = 0; i < (ptrdiff_t)wire.Count(); i++ ) 
  {
    tmax = wire[i]->GetTMax();
    t = wire[i]->GetTMin();
    wire[i]->PointOn( t, p );
    pp.x = (int32)( x0+p.x*xFactor );
    pp.y = (int32)( y0-p.y*yFactor );
    if ( !GetStyle( styles[i].GetColor(), color, width ) ) {
      tool.RestorePen();

      tool.SelectPen( 1/*width*/, GetRValue(color), GetGValue(color), GetBValue(color) );
    }

    tool.MoveTo( pp.x, pp.y );

    i0 = 1;
    while ( i0 > 0 ) {
      t += wire[i]->Step( t, Math::visualSag );
      if (t>tmax-PARAM_REGION) {
        t = tmax;
        i0 = 0;
      }
      wire[i]->PointOn( t, p );
      pp.x = (int32)( x0+p.x*xFactor );
      pp.y = (int32)( y0-p.y*yFactor );
      tool.LineTo( pp.x, pp.y );
    }
  }

  tool.RestorePen();

  POINT p1, p2;
  p1.x = x0; p1.y = y0;
  p2.x = (int)LoUint32(ix); p2.y = (int)LoUint32(iy);
  tool.PutRectangle( p1, p2, RGB(255,255,255), false/*xor*/ );
}
        

//------------------------------------------------------------------------------
// Запись
// ---
#ifndef __USE_QT__
static
void WritePlot( PArray<MbCurve> & wire, SArray<Style> & styles )
{
  TCHAR * name = FileName();
  if ( !name )
    return;

  PRINTDLG pd;
  memset( &pd, 0, sizeof(PRINTDLG) );
  pd.lStructSize = sizeof(PRINTDLG);
  pd.hwndOwner  = ParentForDialog();
  pd.hInstance  = 0;
  pd.Flags = PD_RETURNDC | PD_PRINTTOFILE;
  if (!::PrintDlg( &pd ))
    return;

  if ( pd.hDC ) 
  {
    HDC dc = pd.hDC;
    HGDIOBJ hpen;
    DOCINFO di = { sizeof(DOCINFO), IDS_PRINT_3D_MODEL, name };
    if ( StartDoc(dc,&di) > 0 ) 
    {
      double factor = double( ::GetDeviceCaps(dc,LOGPIXELSX) )/25.4; // pixel_per_mm
      POINT pp;
      MbCartPoint p;
      double t, tmax;
      int style, thick;
      double width = 0.8;
      COLORREF color = RGB(0,0,0);
      ptrdiff_t i, i0;

      style = PS_SOLID;
      thick = (int)( width*factor );
      hpen = HPEN( SelectObject( dc, CreatePen( style, thick, color ) ) );

      for ( i = 0; i < (ptrdiff_t)wire.Count(); i++ ) {
        tmax = wire[i]->GetTMax();
        t = wire[i]->GetTMin();
        wire[i]->PointOn( t, p );
        pp.x = (int32)( p.x*factor );
        pp.y = (int32)(-p.y*factor );

        if ( !GetStyle( styles[i].GetColor(), color, width ) ) {
          DeleteObject( SelectObject( dc, hpen ) );
          thick = (int)( width*factor );
          hpen = HPEN( SelectObject( dc, CreatePen( style, thick, color ) ) );
        }

        MoveToEx( dc, pp.x, pp.y, nullptr );

        i0 = 1;
        while ( i0 > 0 ) {
          t += wire[i]->Step( t, Math::visualSag );

          if ( t > tmax-PARAM_REGION ) {
            t = tmax;
            i0 = 0;
          }

          wire[i]->PointOn( t, p );
          pp.x = (int32)( p.x*factor );
          pp.y = (int32)(-p.y*factor );
          LineTo( dc, pp.x, pp.y );
        }
      }

      DeleteObject( SelectObject( dc, hpen ) );
      ::EndDoc( dc );
    }

    ::DeleteDC( dc );
  }
}
#else // __USE_QT__ 
static
void WritePlot( PArray<MbCurve> &, SArray<Style> & ) {
  PRECONDITION( false ); // Не реализованно
}
#endif // __USE_QT__ 


//------------------------------------------------------------------------------
// Векторный вывод
// ---
void PutPlot( POINT cursorPoint0, POINT cursorPoint1 )
{
  PArray<MbCurve> wire(50,50,true);
  SArray<Style> styles(50,50);
  WorkWindow * wnd = TestVariables::viewManager->GetActiveWindow();
  
  MbCartPoint p0, p1;
  MbVector to;
  MbCartPoint3D p;
  ptrdiff_t i;

  if ( wnd != nullptr ) {
    MbCartPoint3D vista(wnd->GetVista());
    if (vista.z>DELTA_MIN) {
      double xFactor = wnd->GetDrawTool().GetXPixelPerMM();
      double yFactor = wnd->GetDrawTool().GetYPixelPerMM();
      double zFactor = ONE_HALF * ( xFactor + yFactor );
      vista.x /= xFactor;
      vista.y /= yFactor;
      vista.z /= zFactor;
      vista.y = -vista.y;
    }

    //TestVariables::viewManager->GetWire( vista, wire, styles );
    WorkWindow * wind = TestVariables::viewManager->GetActiveWindow();
    if ( wind != nullptr )
      wind->GetWire( vista, wire, styles );

    p0 = wnd->GetPlanePoint( cursorPoint0 );
    p1 = wnd->GetPlanePoint( cursorPoint1 );
    MbRect rect(p0,p1);
    for ( i = wire.MaxIndex(); i >= 0; i-- ) {
      if (!IsFullVisibleInRect( wire[i], rect )) {
         wire.RemoveInd(i);
         styles.RemoveInd(i);
      }
    }

    to.x = - std_min(p0.x, p1.x);
    to.y = - std_max(p0.y, p1.y);
    for ( i = 0; i < (ptrdiff_t)wire.Count(); i++ )
      wire[i]->Move(to);

    double scl = wnd->GetScale();

    if ( fabs(scl - 1) > DELTA_MIN ) {
      MbMatrix matr;
      matr.Scale(scl);
      for ( i = 0; i < (ptrdiff_t)wire.Count(); i++ )
        wire[i]->Transform(matr);
    }
    DrawPlot( wnd, cursorPoint0, cursorPoint1, wire, styles ); // Отрисовка
    if ( GetPoint( IDS_SAVE_TO_FILE, p ) )
      WritePlot( wire, styles ); // Запись

  }
}


//------------------------------------------------------------------------------
// Поднятие флага возможности печати
// ---
void UpRastrFlag() {
  TestVariables::rastrFlag = 1;
}


//------------------------------------------------------------------------------
// Раскрасить объекты в отдельном окне
// ---
void ScanRender( POINT cursorPoint0, POINT cursorPoint1, RPArray<MbGrid> & gridirons,
                 SArray<double> & ambient, SArray<double> & diffuse,
                 SArray<double> & specularity, SArray<double> & shininess,
                 SArray<double> & opacity, SArray<uint32> & colors,
                 PArray<MbItem> & objl )
{
  MbGrid * gr = nullptr;
  ptrdiff_t i, j, l, k;

  WorkWindow *vc = TestVariables::viewManager->GetActiveWindow();
  IGrDraw& tool = vc->GetDrawTool();

  MbMatrix3D into( vc->GetScaleMatrixInto() );
  MbMatrix3D from( vc->GetScaleMatrixFrom() );
  MbCartPoint3D pnt;
  ptrdiff_t tcount = 0; // Количество треугольников
  MbCartPoint3D vista(vc->GetVista());
  double xFactor = tool.GetXPixelPerMM();
  double yFactor = tool.GetYPixelPerMM();
  double zFactor = ONE_HALF * ( xFactor + yFactor );
  MbCartPoint pr0, pr1;
  MbCube cube;
  double zmin, zmax, dz, kz = 0.95; // Затемнение при удаленности
  double ka, kd, ks, kb, kt;
  //MbVector3D norm, light0(lightDirection);
  MbVector3D norm, light0 = tool.GetLight();

  // Установка источника света
  vc->GetDrawTool().GetGLProperty( ka, kd, ks, light0 );
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
  if ( vista.z>DELTA_MIN) {
    for ( j = 0; j < (ptrdiff_t)gridirons.Count(); j++ )
      gridirons[j]->SetVista( vista );
  }

  // Определение габарита изображения
  for ( j = 0; j < (ptrdiff_t)gridirons.Count(); j++ ) {
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

  pr0 = vc->GetPlanePoint( cursorPoint0 );
  pr1 = vc->GetPlanePoint( cursorPoint1 );
  pr0 *= vc->GetScale();
  pr1 *= vc->GetScale();
  MbRect globalGab(pr0,pr1);

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
  PArray <SArray <ptrdiff_t> > boxes( lineXCount*lineYCount, 2, true ); // Множество box для клеток

  SetGridBoxes( gridirons, tcount, globalGab, X_DISCRETE, Y_DISCRETE, discreteXCount, discreteYCount,
                factor, lineXCount, lineYCount, boxes );

  // Отрисовка объектов
  ptrdiff_t nBoxCount;
  uint8 cR, cG, cB;
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
  COLORREF bkColor = RGB(255,255,255);
  BMPContext bmp( (uint16)icount, (uint16)jcount );

//  vc->GetBackground( bkColor );
  pnt.y = globalGab.top;
  pix.y = pix1.y;
  pix1 = pix;

  // Заполнение точек прямоугольного множества цветом фона или цветом растровых объектов
  for ( j = 0; j < jcount; j++ ) {
    pnt.x = globalGab.left;
    pix.x = startx;
    ptrdiff_t/*OV_x64 int */ jLine = j / discreteYCount; // Номер полосы по y
    for ( i = 0; i < icount; i++ ) {
      pnt.z = -MB_MAXDOUBLE;
      pointz = -MB_MAXDOUBLE;
      ptrdiff_t index = -1;
      ptrdiff_t iLine = i / discreteXCount; // Номер полосы по x
      ptrdiff_t nLine = jLine * lineXCount + iLine;
      ptrdiff_t nBox;
      // Работа с растровыми объектами
      if ( gridirons.Count()>0 ) {
        ambs.Flush();
        difs.Flush();
        specs.Flush();
        shins.Flush();
        trans.Flush();
        cols.Flush();
        zbuffer.Flush();
        normals.Flush();
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
            if ( GetVisibleNormal( gr, k, kt<1-DELTA_MIN, delta, pointz, pnt, norm ) ) {
              index = k;      // Признак существования объекта в точке
              ptrdiff_t i3 = 0;
              while ((i3<((ptrdiff_t)zbuffer.Count())) && pointz<zbuffer[i3])
                i3++;
              ambs.InsertInd(i3,ka);
              difs.InsertInd(i3,kd);
              specs.InsertInd(i3,ks);
              shins.InsertInd(i3,kb);
              trans.InsertInd(i3,kt);
              cols.InsertInd(i3,cl);
              zbuffer.InsertInd(i3,pointz);
//
//            
              normals.InsertInd(i3,norm);
            }
          }
        }
      }

      // Определение яркости точки объекта и отрисовка его точки
      if ( index != -1 ) { // Заполнение точек прямоуголного множества цветом растровых объектов
        ztransp0 = 1;
        dColorR = 0;
        dColorG = 0;
        dColorB = 0;
        bool shade = false; // Флаг построения теней
        if ( Math::tempIndex == 1 )
          shade = InShadow( gridirons, pnt, light0 ); // Определение затенённости точки
        for (ptrdiff_t i3=0;i3<(ptrdiff_t)zbuffer.Count();i3++) {
          if (trans[i3]>LENGTH_EPSILON) {
            norm = normals[i3];
            GetColors( light0, ambs[i3], difs[i3], specs[i3], shins[i3], shade,
                       zmin, zmax, kz, dz, zbuffer[i3], norm, (uint32)cols[i3], dR, dG, dB );
            dColorR += dR*ztransp0*trans[i3];
            dColorG += dG*ztransp0*trans[i3];
            dColorB += dB*ztransp0*trans[i3];
          }
          ztransp0 *= 1-trans[i3];
        }
        if (ztransp0>LENGTH_EPSILON) {
          dColorR += (double)GetRValue(bkColor)*ztransp0;
          dColorG += (double)GetGValue(bkColor)*ztransp0;
          dColorB += (double)GetBValue(bkColor)*ztransp0;
        }
        cR = uint8/*BYTE*/( dColorR+0.5 );
        cG = uint8/*BYTE*/( dColorG+0.5 );
        cB = uint8/*BYTE*/( dColorB+0.5 );
//        pp.x = pix.x; pp.y = pix.y;
//        DPtoLP( dc, &pp, 1 );
//        SetPixel( dc, pp.x, pp.y, PALETTERGB( cR, cG, cB ) );
        bmp += RGB( cR, cG, cB );
      }
      else { // Заполнение точек прямоуголного множества цветом фона 
        bmp += bkColor;
      }
      pnt.x += X_DISCRETE;
      pix.x += 1;
    }
    pnt.y -= Y_DISCRETE;
    pix.y += 1;

    if ( PeekEscape() )
        break; // Если нажата клавиша, прекратить отрисовку

  }

  // Перевод точек и нормалей триангуляции в исходное состояние
  if ( vista.z>DELTA_MIN) {
    for ( j=0; j<(ptrdiff_t)gridirons.Count(); j++ )
      gridirons[j]->DeleteVista( vista );
  }
  for ( i=0; i<(ptrdiff_t)gridirons.Count(); i++ )
    gridirons[i]->Transform( from );  // Перевод сетки

  // Работа с полигонами векторных объектов 
  MbMesh mesh;
  double sag = Math::visualSag; // / vc->scale;
  MbRegDuplicate * iReg = nullptr;
  MbAutoRegDuplicate autoReg( iReg );  

  for ( k=0; k<(ptrdiff_t)objl.Count(); k++ ) {
    mesh.Flush();
    MbItem * meshItem = objl[k]->CreateMesh( MbStepData(ist_SpaceStep, sag), MbFormNote(true, false, true, TestVariables::exactMesh), iReg );
    if ( (meshItem != nullptr) && (meshItem->IsA() == st_Mesh) )
      mesh.AddMesh( (MbMesh &)*meshItem, false );
    if ( mesh.IsComplete() ) {
/*
     // Используем пикселные полигоны и пикселную сетку
      MapMesh *newMap = new MapMesh();
      // Перевод пространственной сетки полигонов в пикселную сетку
      for ( i = 0; i < mesh.Count(); i++ ) {
        MbPolygon3D * pl = mesh.GetPolygon( i );
        MapPolygon * poly = new MapPolygon();
        l = 0;
        for ( j = 0; j < pl->Count(); j++ ) {
          pl->GetPoint( j, pnt );
          wp.InitPoint( pnt, vc->GetOwnMatrixInto(), vc->GetVista() );
          wp.x = wp.x - pix1.x;
          wp.y = wp.y - pix1.y;
          if ( wp.x<0 || wp.x>=icount || wp.y<0 || wp.y>=jcount ) {
            l++;
            break;
          }  
          poly->Add( wp );
        }
        if ( l==0 )
          newMap->Add( poly );
        else
          delete poly;
      }

      // Заполнение точек по алгоритму Брезенхема
      int width = objl[k]->GetWidth(); // Толщина линий
      COLORREF rgbc = objl[k]->GetColor();
      cR = GetRValue( rgbc );
      cG = GetGValue( rgbc );
      cB = GetBValue( rgbc );
      for ( i = 0; i < newMap->GetMapPolyCount(); i++ ) {
        int x0, x1, y0, y1, m;
        double t, d, s;
        MapPolygon * poly = newMap->GetMapPolygon( i );
        if (poly==nullptr)
          continue;
        j = 0;
        wp = poly->GetMapPoint( j );
        x0 = (int)wp.x;
        y0 = (int)wp.y;
        for ( j = 1; j < poly->GetMapPointCount(); j++ ) {
          wp = poly->GetMapPoint( j );
          x1 = (int)wp.x;
          y1 = (int)wp.y;
          if ( ::abs_t(x1-x0)>=::abs_t(y1-y0) && x1!=x0 ) {
            t = double(y1-y0)/double(x1-x0);
            if (x1>x0) {
              for (l=x0; l<=x1; l++) {
                d = t*(l-x0);
                s = (d>=0)? 0.5: -0.5;
                m = y0+int(d+s);
                bmp.Set( (uint16)l, (uint16)m, cR, cG, cB );
                for (int iw=1;iw<width;iw++) { // Утолщение участка линии
                  m = y0+int(d+s+0.5*iw);
                  bmp.Set( (uint16)l, (uint16)m, cR, cG, cB );
                  m = y0+int(d+s-0.5*iw);
                  bmp.Set( (uint16)l, (uint16)m, cR, cG, cB );
                }
              }  
            }
            else {
              for (l=x0; l>=x1; l--) {
                d = t*(l-x0);
                s = (d>=0)? 0.5: -0.5;
                m = y0+int(d+s);
                bmp.Set( (uint16)l, (uint16)m, cR, cG, cB );
                for (int iw=1;iw<width;iw++) { // Утолщение участка линии
                  m = y0+int(d+s+0.5*iw);
                  bmp.Set( (uint16)l, (uint16)m, cR, cG, cB );
                  m = y0+int(d+s-0.5*iw);
                  bmp.Set( (uint16)l, (uint16)m, cR, cG, cB );
                }
              }  
            }
            x0 = x1;
            y0 = y1;
          }
          else {
            if ( ::abs_t(x1-x0)<=::abs_t(y1-y0) && y1!=y0 ) {
              t = double(x1-x0)/double(y1-y0);
              if (y1>y0) {
                for (m=y0; m<=y1; m++) {
                  d = t*(m-y0);
                  s = (d>=0)? 0.5 : -0.5;
                  l = x0+int(d+s);
                  bmp.Set( (uint16)l, (uint16)m, cR, cG, cB );
                  for (int iw=1;iw<width;iw++) { // Утолщение участка линии
                    l = x0+int(d+s+0.5*iw);
                    bmp.Set( (uint16)l, (uint16)m, cR, cG, cB );
                    l = x0+int(d+s-0.5*iw);
                    bmp.Set( (uint16)l, (uint16)m, cR, cG, cB );
                  }
                }  
              }
              else {
                for (m=y0; m>=y1; m--) {
                  d = t*(m-y0);
                  s = (d>=0)? 0.5 : -0.5;
                  l = x0+int(d+s);
                  bmp.Set( (uint16)l, (uint16)m, cR, cG, cB );
                  for (int iw=1;iw<width;iw++) { // Утолщение участка линии
                    l = x0+int(d+s+0.5*iw);
                    bmp.Set( (uint16)l, (uint16)m, cR, cG, cB );
                    l = x0+int(d+s-0.5*iw);
                    bmp.Set( (uint16)l, (uint16)m, cR, cG, cB );
                  }
                }  
              }
              x0 = x1;
              y0 = y1;
            }
          }
        }
      }
      delete newMap;
*/    
      // Не используем пикселные полигоны и пикселную сетку
      int width = objl[k]->GetWidth(); // Толщина линий
      COLORREF rgbc = (COLORREF)objl[k]->GetColor();
      cR = GetRValue( rgbc );
      cG = GetGValue( rgbc );
      cB = GetBValue( rgbc );

      // Перевод пространственных точек сетки полигонов в пикселы
      for ( i = 0; i < (ptrdiff_t)mesh.PolygonsCount(); i++ ) {
        MbPolygon3D * pl = mesh.SetPolygon( i );
        if ( (pl==nullptr) || (pl->Count()==0) )
          continue;
        ptrdiff_t x0, x1, y0, y1;
        ptrdiff_t m;
        double t, d, s;
        j = 0;
        pl->GetPoint( j, pnt ); // pnt - пространственная точка
        POINT wp;
        wp = vc->InitPoint( pnt, vc->GetOwnMatrixInto(), vc->GetVista() ); // wp - пикселная точка
        wp.x = wp.x - pix1.x;
        wp.y = wp.y - pix1.y;
        if ( wp.x < 0 || (ptrdiff_t)wp.x >= icount || wp.y < 0 || (ptrdiff_t)wp.y >= jcount ) {
          continue;
        }  
        // Локальные координаты пиксела
        x0 = (ptrdiff_t)wp.x;
        y0 = (ptrdiff_t)wp.y;

        // Заполнение точек по алгоритму Брезенхема
        for ( j = 1; j < (ptrdiff_t)pl->Count(); j++ ) {
          pl->GetPoint( j, pnt ); // pnt - пространственная точка
          wp = vc->InitPoint( pnt, vc->GetOwnMatrixInto(), vc->GetVista() ); // wp - пикселная точка
          wp.x = wp.x - pix1.x;
          wp.y = wp.y - pix1.y;
          if ( (ptrdiff_t)wp.x < 0 || (ptrdiff_t)wp.x >= (ptrdiff_t)icount || (ptrdiff_t)wp.y < 0 || (ptrdiff_t)wp.y >= (ptrdiff_t)jcount ) {
            continue;
          }  
          // Локальные координаты пиксела
          x1 = (ptrdiff_t)wp.x;
          y1 = (ptrdiff_t)wp.y;
          if ( ::abs_t(x1-x0)>=::abs_t(y1-y0) && x1!=x0 ) {
            t = double(y1-y0)/double(x1-x0);
            if ( x1 > x0 ) {
              for ( l = (ptrdiff_t)x0; l <= (ptrdiff_t)x1; l++ ) {
                d = t * (double)(l-x0);
                s = (d>=0)? 0.5: -0.5;
                m = y0 + ptrdiff_t(d + s);
                bmp.Set( (uint16)l, (uint16)m, cR, cG, cB );
                for (ptrdiff_t iw = 1; iw < (ptrdiff_t)width; iw++ ) { // Утолщение участка линии
                  m = y0 + ptrdiff_t(d + s + 0.5*(double)iw );
                  bmp.Set( (uint16)l, (uint16)m, cR, cG, cB );

                  m = y0 + ptrdiff_t(d + s - 0.5*(double)iw );
                  bmp.Set( (uint16)l, (uint16)m, cR, cG, cB );
                }
              }  
            }
            else {
              for ( l = (ptrdiff_t)x0; l >= (ptrdiff_t)x1; l-- ) {
                d = t * (double)(l - x0);
                s = (d>=0)? 0.5: -0.5;
                m = y0 + ptrdiff_t(d + s);
                bmp.Set( (uint16)l, (uint16)m, cR, cG, cB );
                for ( ptrdiff_t iw = 1; iw < (ptrdiff_t)width; iw++ ) { // Утолщение участка линии
                  m = y0 + ptrdiff_t(d + s + 0.5 * (double)iw);
                  bmp.Set( (uint16)l, (uint16)m, cR, cG, cB );

                  m = y0 + ptrdiff_t(d + s - 0.5 * (double)iw);
                  bmp.Set( (uint16)l, (uint16)m, cR, cG, cB );
                }
              }  
            }
            x0 = x1;
            y0 = y1;
          }
          else {
            if ( ::abs_t(x1-x0)<=::abs_t(y1-y0) && y1!=y0 ) {
              t = double(x1-x0)/double(y1-y0);
              if (y1>y0) {
                for ( m = y0; m <= y1; m++ ) {
                  d = t * (double)(m - y0);
                  s = (d>=0)? 0.5 : -0.5;
                  l = (ptrdiff_t)( x0 + ptrdiff_t(d + s) );
                  bmp.Set( (uint16)l, (uint16)m, cR, cG, cB );
                  for (ptrdiff_t iw = 1; iw < (ptrdiff_t)width; iw++ ) { // Утолщение участка линии
                    l = (ptrdiff_t)( x0 + ptrdiff_t(d + s + 0.5 * (double)iw) );
                    bmp.Set( (uint16)l, (uint16)m, cR, cG, cB );

                    l = (ptrdiff_t)( x0 + ptrdiff_t(d + s - 0.5 * (double)iw) );
                    bmp.Set( (uint16)l, (uint16)m, cR, cG, cB );
                  }
                }  
              }
              else {
                for ( m = y0; m >= y1; m-- ) {
                  d = t * (double)(m - y0);
                  s = (d>=0)? 0.5 : -0.5;
                  l = (ptrdiff_t)( x0 + ptrdiff_t(d + s) );
                  bmp.Set( (uint16)l, (uint16)m, cR, cG, cB );
                  for ( ptrdiff_t iw = 1; iw < (ptrdiff_t)width; iw++ ) { // Утолщение участка линии
                    l = (ptrdiff_t)( x0 + ptrdiff_t(d + s + 0.5 * (double)iw) );
                    bmp.Set( (uint16)l, (uint16)m, cR, cG, cB );

                    l = (ptrdiff_t)( x0 + ptrdiff_t(d + s - 0.5 * (double)iw) );
                    bmp.Set( (uint16)l, (uint16)m, cR, cG, cB );
                  }
                }  
              }
              x0 = x1;
              y0 = y1;
            }
          }
        }
      }
//
    }
  }
    
//  //bmp.CopyFromDC( tool.GetHDC(), (uint16)pix1.x, (uint16)pix1.y );
//  bmp.CopyFromDC( (uint16)pix1.x, (uint16)pix1.y );

  // Отрисовка рамки области
  int x_ = std_min( cursorPoint0.x, cursorPoint1.x );
  int y_ = std_min( cursorPoint0.y, cursorPoint1.y );
  int ix = ::abs_t(cursorPoint0.x - cursorPoint1.x) + 1;
  int iy = ::abs_t(cursorPoint0.y - cursorPoint1.y) + 1;
  if (x_>0) x_--;
  if (y_>0) x_--;

  POINT p1;  p1.x = x_; p1.y = y_;
  POINT p2;  p2.x = ix; p2.y = iy;
  tool.PutRectangle( p1, p2, RGB(255,255,255), false/*xor*/);

  if ( GetPoint( IDS_SAVE_TO_FILE, pr0 ) ) {
    TCHAR* name = FileName();
    if ( name )
      bmp.Save( name );
  }
}


//------------------------------------------------------------------------------
// Растровый вывод
// ---
void PutRastr( POINT cursorPoint0, POINT cursorPoint1 )
{
  PArray <MbItem> objs(50,50,false);
  PArray <MbItem> objl(50,50,false);
  RPArray <MbGrid> gridirons(5,1);
  SArray <double> ambient(5,1);
  SArray <double> diffuse(5,1);
  SArray <double> specularity(5,1);
  SArray <double> shininess(5,1);
  SArray <double> opacity(5,1);
  SArray <uint32> solors(5,1);
  float am = (float)0.4;  // Общий фон
  float df = (float)0.3;  // Диффузия
  float sp = (float)0.3;  // Зеркальность
  float sh = (float)25;   // Блеск
  float tr = (float)1.0;  // Непрозрачность
  float em = (float)0.0;  // Излучение
  uint32 cl;
  double sag = Math::visualSag; // / vc->scale;

  SetWaitCursor( true );
  TestVariables::viewManager->GetRenderingObjects( objs );
  TestVariables::viewManager->GetLineDrawingObjects( objl );
  for ( ptrdiff_t i = 0; i < (ptrdiff_t)objs.Count(); i++ ) {
    size_t gridironsCount = gridirons.Count();
    ::AddGrid( objs[i], sag, gridirons ); 
    cl = objs[i]->GetColor();
    objs[i]->GetVisual( am, df, sp, sh, tr, em );
    for ( size_t j = gridironsCount; j < gridirons.Count(); j++ ) {
      ambient.Add(am);
      diffuse.Add(df);
      specularity.Add(sp);
      shininess.Add(sh);
      opacity.Add(tr);
      solors.Add(cl);
    }

    if ( PeekEscape() )
    {
      SetWaitCursor( false );
      return; // Если нажата клавиша, прекратить отрисовку
    }
  }

  if ( gridirons.Count()>0 || objl.Count()>0 ) {
    if ( Math::tempIndex == 2 )
      ChangeNormals( gridirons ); // Изменить направление нормалей сетки случайным образом
    ScanRender( cursorPoint0, cursorPoint1, gridirons, ambient, diffuse, specularity, shininess, opacity, solors, objl );
  }

  for ( size_t i = 0, gridsCount = gridirons.Count(); i < gridsCount; i++ ) 
    delete gridirons[i];

  SetWaitCursor( false );
}


//------------------------------------------------------------------------------
// Векторный вывод в растр
// ---
void PutVect( POINT cursorPoint0, POINT cursorPoint1 ) {
  PArray<MbItem> objs(50,50,false);
  PArray<MbItem> objl(50,50,false);
  RPArray<MbGrid> gridirons(5,1);
  SArray<double> ambient(5,1);
  SArray<double> diffuse(5,1);
  SArray<double> specularity(5,1);
  SArray<double> shininess(5,1);
  SArray<double> opacity(5,1);
  SArray<uint32> solors(5,1);

  SetWaitCursor( true );
  TestVariables::viewManager->GetRenderingObjects( objl );
  TestVariables::viewManager->GetLineDrawingObjects( objl );

  if ( gridirons.Count()>0 || objl.Count()>0 ) {
    ScanRender( cursorPoint0, cursorPoint1, gridirons, ambient, diffuse, specularity, shininess, opacity, solors, objl );
  }
  SetWaitCursor( false );
}

