// Пустая строка необходима для ресурсного компилятора MSVC

#ifndef __TEST_H
#define __TEST_H

//#define __NATIVE_LANGUAGE__ // Native language have to defined in project (CMake) settings

//------------------------------------------------------------------------------
// Базовые идентификаторы
//---
#define CM_MAIN_BASE                      15000 //  Базовый идентификатор для главных команд приложения (от главного меню)
#define WARNING_BASE                      20000 //  Базовый идентификатор для сообщений об ошибках
#define STRING_BASE                       25000 //  Базовый идентификатор для стрингов


//------------------------------------------------------------------------------
// Базовые идентификаторы
//---
#define ZOOMIN                            1
#define ZOOMOUT                           2
#define SETORIGIN                         3

#define IDS_YES                           1
#define IDS_NO                            2


//------------------------------------------------------------------------------
// Кнопки
// ---
#define IDC_PUSHBUTTON_SPLINE             501
#define IDC_PUSHBUTTON_SEGMENT            502
#define IDC_PUSHBUTTON_POLYLINE           503
#define IDC_PUSHBUTTON_ARC                504
#define IDC_PUSHBUTTON_CLOSE              505
#define IDC_PUSHBUTTON_EXIT               506
#define IDC_PUSHBUTTON_CHANGE             531
#define IDC_PUSHBUTTON_STEPBACK           532

//------------------------------------------------------------------------------
// Акселераторы
//---
#define ACCELERATORS_BASE                 1

//------------------------------------------------------------------------------
// Диалоги
//---
#define IDD_DIALOG_BASE                   10001
#define IDD_DIALOG_ENTER_SPACEPOINT       10002
#define IDD_DIALOG_SELECT_PROJECTION      10004
#define IDD_DIALOG_ENTER_CONTOUR          10005
#define IDD_DIALOG_ENTER_DEGREE           10006
#define IDD_DIALOG_ENTER_BOOLEAN          10007
#define IDD_DIALOG_CHAMFER_PARAMETERS     10008

#define IDD_DIALOG_SELECT_DIRECTION       10010
#define IDD_DIALOG_ENTER_PLANEPOINT       10011
#define IDD_DIALOG_LICENSE                10012


#define BUTTON_SET                        10000
#define BUTTON_SET_EXIT                   10001
#define BUTTON_EXIT                       10002
#define H_LIST                            10003
#define H_EDIT                            10004
#define H_CHILDLIST                       10008
#define H_STATIC                          10009
#define BUTTON_SELECT                     10005
#define BUTTON_SELECT_CHILD               10012
#define BUTTON_APPLY                      10013
#define BUTTON_FILTER                     10014
#define BUTTON_CATALOG                    10015
#define BUTTON_SELECT_ALL                 10016
#define BUTTON_SELECT_TYPE                10017
#define BUTTON_SET_TYPE_VALUE             10018

//------------------------------------------------------------------------------
// Элементы управления
//--
#define IDC_TEXT                          200
#define IDC_EDIT1                         201
#define IDC_EDIT2                         202
#define IDC_EDIT3                         203
#define IDC_EDIT4                         204
#define IDC_EDIT5                         205
#define IDC_EDIT6                         206
#define IDC_EDIT7                         207
#define IDC_EDIT8                         208
#define IDC_CHAMFERALLEQUAL               209 // CheckBox в диалоге фаски

#define IDC_STATIC                        210
#define IDC_X0Y                           211
#define IDC_Z0X                           212
#define IDC_Y0Z                           213
#define IDC_IZO                           214

#define IDC_STATIC_KEY                    220
#define IDC_STATIC_SIG                    221
#define IDC_EDIT_KEY                      222
#define IDC_EDIT_SIG                      223
#define IDC_CHECK_FILE                    224


#define IDC_TEXTC                         300


#define IDD_TIMETESTRESULT                26008
#define IDC_TIMETESTRESULT_TEXT           26009
#define IDC_TIMETESTRESULT_LV             26010

//------------------------------------------------------------------------------
// Иконы
//---
#define IDI_ICON_1                        1
#define IDI_ICON_2                        2
#define IDI_ICON_3                        3
#define IDI_ICON_4                        4
#define IDI_ICON_5                        5


//------------------------------------------------------------------------------
//  Меню
// ---
#define IDM_MENU_1                        1


//------------------------------------------------------------------------------
// Основные команды приложения
// Начиная с CM_MAIN_BASE 15000
// ---
#define CM_OFFSETSHELLBYFACES             15001
#define CM_LOFTEDSHELLBYPOINTS            15002
#define CM_LOFTEDSHELLBYCURVES            15003
#define CM_NEWCURVEEXTRUSIONSHELL         15004
#define CM_NEWCURVEREVOLUTIONSHELL        15005
#define CM_NEWCURVEEVOLUTIONSHELL         15006
#define CM_NEWCURVELOFTEDSHELL            15007
#define CM_NEWCURVELOFTEDSHELL_CL         15008
#define CM_SHELLOFSOLID                   15009
#define CM_CUTSHELL_BYCONTOUR             15010
#define CM_CUTSHELL_BYSURFACE             15011
#define CM_NEWCURVEEXTRUSIONSOLID         15012
#define CM_NEWCURVEREVOLUTIONSOLID        15013
#define CM_NEWCURVEEVOLUTIONSOLID         15014
#define CM_NEWCURVELOFTEDSOLID            15015
#define CM_NEWCURVELOFTEDSOLID_CL         15016
#define CM_NEWUNIONSOLID                  15017
#define CM_BOSSCURVEEXTRUSIONSOLID        15018
#define CM_BOSSCURVEREVOLUTIONSOLID       15019
#define CM_BOSSCURVEEVOLUTIONSOLID        15020
#define CM_BOSSCURVELOFTEDSOLID           15021
#define CM_BOSSCURVELOFTEDSOLID_CL        15022
#define CM_BOSSUNIONSOLID                 15023
#define CM_CUTCURVEEXTRUSIONSOLID         15024
#define CM_CUTCURVEREVOLUTIONSOLID        15025
#define CM_CUTCURVEEVOLUTIONSOLID         15026
#define CM_CUTCURVELOFTEDSOLID            15027
#define CM_CUTCURVELOFTEDSOLID_CL         15028
#define CM_CUTUNIONSOLID                  15029
#define CM_INTERSECTCURVEEXTRUSIONSOLID   15030
#define CM_INTERSECTCURVEREVOLUTIONSOLID  15031
#define CM_INTERSECTCURVEEVOLUTIONSOLID   15032
#define CM_INTERSECTCURVELOFTEDSOLID      15033
#define CM_INTERSECTCURVELOFTEDSOLID_CL   15034
#define CM_INTERSECTUNIONSOLID            15035
#define CM_SOLIDCHANNEL                   15036
#define CM_NEWSPIRALSURFACE               15037
#define CM_NEWRIBSHELL                    15039
#define CM_SOLIDNPDRAFT                   15040
#define CM_REBUILD_ITEM                   15041
#define CM_REBUILD_SHELL                  15042
#define CM_NEWOFFSETEXTENDED              15043
#define CM_NEWPLANECURVEBYSPACE           15044
#define CM_NEW_WIREFRAME_BY_CURVES        15045
#define CM_NEW_WIREFRAME_BY_POINTS        15046
#define CM_CONVERTNURBSSURFACEINTOOPEN    15047
#define CM_CONVERTNURBSSURFACEINTOCLOSE   15048
#define CM_CONVERTPLANENURBSINTOOPEN      15049
#define CM_CONVERTPLANENURBSINTOCLOSE     15050
#define CM_TOPOITEMS_MINDISTANCE          15051
#define CM_NEWCURVE3DREVOLUTIONSOLID      15052
#define CM_NEWCURVE3DEXTRUSIONSOLID       15053
#define CM_CONVERTNURBSINTOOPEN           15054
#define CM_CONVERTNURBSINTOCLOSE          15055
#define CM_NEWCUBICSPLINE3D               15056
#define CM_CHECK_SHELL_GEOMETRY           15057
#define CM_CHECK_SHELL_NAMES              15058
#define CM_SETLIGHTDIRECTION              15059
#define CM_TRANSLATECURVE3DTOCURVE        15060
#define CM_NEWNURBSSURFACESHELL           15061
#define CM_FACESFILLET                    15062
#define CM_NEWRIBSOLID                    15063
#define CM_NEW_SPLIT_SOLID                15064 //"Разбить грани тела движением контура"
#define CM_SETVIEWPLACEMENT               15065
#define CM_NEWFACESINTERSECTION           15066
#define CM_UVMESHCOUNT                    15067
#define CM_GLPROPERT                      15068
#define CM_GLAMBIENT                      15069
#define CM_GLDIFFUSE                      15070
#define CM_GLSPECULARITY                  15071
#define CM_GLSHININESS                    15072
#define CM_GLOPACITY                      15073
#define CM_GLEMISSION                     15074
#define CM_DELTA_ANGLE                    15075
#define CM_FIRSTCHILD                     15077
#define CM_DELTA_RANGE                    15078
#define CM_NEWFACECONTOUR                 15079
#define CM_PRINTBITMAP                    15080
#define CM_MAKESPACEWIREFRAME             15081
#define CM_MAKEPLANEWIREFRAME             15082
#define CM_MAKEWIREFRAMEONPLANE           15083
#define CM_MAKETRIANGLEPOINTFRAME         15084
#define CM_MAKETRIANGLEWIREFRAME          15085
#define CM_MAKETRIANGLEPLANE              15086
#define CM_SETBACKGRCOLOR                 15087
#define CM_GETFACECENTRE                  15088
#define CM_CUTMESH_BYPLANE                15089
#define CM_GETEDGEFACESANGLE              15090
#define CM_EDGEFACESANGLE                 15091
#define CM_SOLIDFACEMAP                   15092
#define CM_FILEEXPORT                     15093
#define CM_XT_READ                        15094 // Прочитать X_T
#define CM_XT_WRITE                       15095 // Записать X_T
#define CM_STEP_READ                      15096 // Прочитать STEP
#define CM_STEP_WRITE                     15097 // Записать STEP
#define CM_CREATECURVEISOPOINTS           15098
#define CM_GETSOLIDMASSINERTIA            15099
#define CM_GETCONTOURAREA                 15100
#define CM_DISTANCEALONG                  15101
#define CM_GLRENDERCOLOR                  15102
#define CM_PROP_OBJ                       15103
#define CM_NEWSIMPLEXSURFACE              15104
#define CM_NEWTRIBEZIERSURFACE            15105
#define CM_CURVE_B_SPLINE                 15106
#define CM_NEWRECTANGLENURBS              15107
#define CM_SURFACETESTA                   15108
#define CM_CURVETESTA                     15109
#define CM_NEWEDGESCURVE                  15110
#define CM_NEWFACEPROJECTION              15111
#define CM_MAKEOFFSETTHINCONTOUR          15112
#define CM_MAKESLOPECONTOUR               15113
#define CM_MAKEOFFSETCONTOUR              15114
#define CM_APPARCCURVE                    15115
#define CM_TRIANGULATEFACE                15116
#define CM_FILETAKE                       15117
#define CM_FILEOPENREAD                   15118
#define CM_RECONSTRUCTCONTOURS            15119
#define CM_OUTERCONTOURS                  15120
#define CM_ENCLOSECONTOURS                15121
#define CM_SAT_READ                       15122 // "Читать SAT",
#define CM_SAT_WRITE                      15123 // "Писать SAT",
#define CM_IGES_READ                      15124 // "Читать IGES",
#define CM_IGES_WRITE                     15125 // "Писать IGES",
#define CM_STL_WRITE                      15126 // "Писать STL",
#define CM_VRML_WRITE                     15127 // "Писать VRML",
#define CM_MOVECPLANE                     15128
#define CM_NEWCURVEEXPANCEDSOLID          15129
#define CM_NEWLOFTEDSOLID                 15130
#define CM_POPUPITEM1                     15131
#define CM_POPUPITEM2                     15132
#define CM_STL_READ                       15133 // "Читать STL"
#define CM_SETVIEWPOINT                   15134
#define CM_PTORIGIN                       15135
#define CM_VRML_READ                      15136 // "Читать VRML"
#define CM_PARAMTRIANGULATEFACE           15137
#define CM_NEWSOLIDEXTRUSIONSOLID         15138
#define CM_NEWSOLIDEVOLUTIONSOLID         15139
#define CM_NEWSOLIDREVOLUTIONSOLID        15140
#define CM_SETPPLANEDIRECTION             15141
#define CM_NEWELLIPSEARC                  15143
#define CM_NEWEXPANSION                   15145
#define CM_ALL_EMBRACING_CNTRS            15146
#define CM_NEWNURBSCONIC                  15147
#define CM_NEWANYSURFACE                  15148
#define CM_CURVATURE_PLOT                 15149
#define CM_NEWPLANE                       15150
#define CM_NEWPLANE3POINTS                15151
#define CM_REBUILD_ALL_SHELLS             15152
#define CM_CUTMESH_BYLINE                 15153
#define CM_MESH_SECTION                   15154
#define CM_MESH_MESH_INTERSECTION         15155
#define CM_NEWTORUS3POINTS                15156
#define CM_CONJUGATE_CONTOUR_SEGMENTS     15157
#define CM_NEWSPHERE3POINTS               15158
#define CM_GETEDGEEDGEANGLE               15159
#define CM_GETMESHINERTIA                 15160

#define CM_NEWCYLINDER3POINTS             15161
#define CM_CHECK_SURFACE_DERIVES          15162
#define CM_SOLIDSMOOTH                    15163
#define CM_NEWMESH_BY_UNION               15164
#define CM_NEWMESH_BY_INTERSECT           15165
#define CM_NEWMESH_BY_SUBSTRACT           15166

#define CM_CHECK_MESH_CLOSENESS           15167

#define CM_SETPLANEPLANEANGLE             15171

#define CM_NEWCONE3POINTS                 15186
#define CM_MANUFACTURING                  15187
#define CM_PRINTPLOTT                     15188
#define CM_SOLIDMAP                       15189
#define CM_SURFACEMAP                     15190
#define CM_CURVEMAP                       15191
#define CM_SOLIDRENDERING                 15192
#define CM_SURFACERENDERING               15193
#define CM_RENDERING                      15194
#define CM_TRIANGULATESOLID               15195
#define CM_TRIANGULATESURFACE             15196
#define CM_TRIANGULATEPOINTCLOUD          15197
#define CM_CURVESMAP                      15198

#define CM_SOLIDHIDE                      15200
#define CM_SURFACEHIDE                    15201
#define CM_SOLIDGABARIT                   15202
#define CM_SURFACEGABARIT                 15203
#define CM_CURVEGABARIT                   15204
#define CM_FACEHIDE                       15205

#define CM_EDITREDRAW                     15206

#define CM_SPLINE_EQUID                   15208
#define CM_NEWLINESEGDMIN                 15209
#define CM_NEWSEGMENTPOINTPOINT           15210
#define CM_REGULARPOLYGON                 15211
#define CM_RECTANGLE                      15212
#define CM_NEWPOLYLINE                    15213
#define CM_NEWCONTOUR                     15214
#define CM_NEWCONTOUR3D                   15215
#define CM_NEWEXTRUSION                   15216
#define CM_NEWREVOLUTION                  15217
#define CM_NEWEVOLUTION                   15218
#define CM_NEWRULEDSURFACE                15219
#define CM_NEWSECTORSURFACE               15220

#define CM_NEWARC3DBYPOINTS               15222
#define CM_NEWCIRCLE3DBYPOINTS            15223
#define CM_NEWCIRCLE3DBYCENTRE            15224
#define CM_EDITOWNPLANEROTATE             15225
#define CM_DELETECONSTRUCTIVEPLANE        15226
#define CM_ROTATEPLANEINVIEW              15227
#define CM_TRANSFORMPLANEOBJECT           15228

#define CM_RECOGNITIONFILLETFACES         15229 // Показать все грани скругления в теле.
#define CM_RECOGNITIONSPIRALSURFACES      15230 // Показать все спиральные поверхности в теле.
#define CM_FACEGABARIT                    15231
#define CM_ADD_VERTEXEDGES_AS_CURVES      15232 // Показать все ребра, ствкующиеся в вершине, добавив в модель копии кривых.
#define CM_ADD_SOLIDEDGES_AS_CURVES       15233 // Показать все ребра тела, добавив в модель копии кривых.
#define CM_ADD_SOLIDVERTS_AS_POINTS       15234 // Показать все вершины тела, добавив в модель копии точек.
#define CM_SELECTCURVEBOUNDEDSURFACE      15235
#define CM_SELECTOFFSETSURFACE            15236

#define CM_SELECT_ALL_SOLIDS              15237
#define CM_SELECT_CLOSED_SOLIDS           15238
#define CM_SELECT_UNCLOSED_SOLIDS         15239

#define CM_SELECTPOLYSURFACE              15240
#define CM_SELECTSWEPTSURFACE             15241
#define CM_SELECTALLSURFACES              15242
#define CM_SELECTELEMENTARYSURFACE        15243
#define CM_SELECTPLANE                    15244
#define CM_SELECTCONTOURS2D               15245

#define CM_SELECTCONTOURSONSURFACE        15246
#define CM_SELECTCONTOURS3D               15247
#define CM_SELECTINTERSECTIONCURVE        15248

#define CM_SELECTALLCURVES                15250
#define CM_SELECTPOLYCURVE                15251

#define CM_SELECTLINE                     15253
//#define CM_SELECTCPLANEPOINTS             15254

#define CM_SELECTALLPOINTS                15256
//#define CM_SELECTPOINTS                   15257
#define CM_SELECTBYCPLANE                 15258
#define CM_EDITREVERSSELECTALL            15259
#define CM_EDITUNSELECTALL                15260
#define CM_EDITSELECTALL                  15261
#define CM_EDITSCALESELECTED              15262
#define CM_EDITLINEROTATE                 15263
#define CM_EDITPLANEROTATE                15264

#define CM_EDITMOVE                       15266

#define CM_EDITPROPERTY                   15272
#define CM_TRANSLATETOBASIS               15273
#define CM_TRANSLATETOINGOT               15274
#define CM_DELETEOBJECT                   15275
#define CM_MIRRORDUPLICATEOBJECT          15276
#define CM_DUPLICATEOBJECT                15277
#define CM_SCALEOBJECT                    15278
#define CM_LINEROTATEOBJECT               15279
#define CM_PLANEROTATEOBJECT              15280
#define CM_TRANSFORMOBJECT                15281
#define CM_MOVEOBJECT                     15282
#define CM_SOLIDFILLETSSETLASTVERS        15283

#define CM_ADDBASISPOINTS                 15287
#define CM_CUTMODELOBJECTS                15288
#define CM_TRANSLATECONTOURTOCONTOURON    15289
#define CM_DISASSEMBLY                    15290
#define CM_INSERTPOINTINCONTOUR           15291
#define CM_CONTOURSENSE                   15292
#define CM_TRANSLATECURVETOCONTOURON      15293
#define CM_TRANSLATECURVETOCURVE3D        15294
#define CM_SPLINEINSERTKNOT               15295
#define CM_SETDEGREE                      15296
#define CM_SPLINECHANGEPOINT              15297
#define CM_SPLINETRIMM                    15298
#define CM_SPLINENURBS                    15299
#define CM_SPLINETANGENT                  15300
#define CM_SPLINENEW_POINT                15301
#define CM_SPLINESET_WEIGHTS              15302
#define CM_BREAKCURVE                     15303
#define CM_NEWTRIMMEDCURVE                15304
#define CM_DELETEPARTP1P2                 15305
#define CM_DELETEPART                     15306
#define CM_SPLINEINVERSE                  15307
#define CM_SPLINESETCLOSED                15308
#define CM_SOLIDCREATORDELETE             15309
#define CM_SOLIDCREATORSETLASTVERS        15310

#define CM_SOLIDENDCUT                    15311
#define CM_TRANSLATESOLIDTONURBS          15312
#define CM_TRANSLATESOLIDTOBASIS          15313
#define CM_TRANSLATESOLIDTOSURFACES       15314
#define CM_TRANSLATESOLIDTOFACES          15315
#define CM_REBUILD_SHELL_WITHCOPYMODE     15316
#define CM_BREAKSOLIDONPARTS              15317
#define CM_SOLIDENDMOVE                   15318
#define CM_CREATESHEETSOLID               15319
#define CM_SHEETSOLIDPLATE                15320
#define CM_SHEETSOLIDHOLE                 15321
#define CM_BENDBYEDGES                    15322
#define CM_BENDOVERSEGMENT                15323
#define CM_BEND                           15324
#define CM_UNBEND                         15325
#define CM_TRANSLATESURFACETONURBS        15326
#define CM_TRANSLATESURFACETOBASIS        15327
#define CM_SOLIDFINISH                    15328
#define CM_GOSURFACETHROUGHPLANE          15329
#define CM_GOSURFACETHROUGHPOINT          15330
#define CM_TRANSLATECURVETONURBS          15331
#define CM_TRANSLATECURVETOBASIS          15332
#define CM_CURVEADDPOINT                  15333
#define CM_MOVECURVEPOINT                 15334
#define CM_BEND_ANY                       15335
#define CM_SIMPLIFY_SHEET_PATTERN         15336
#define CM_CREATEBREAKPARTS               15337
#define CM_CREATEMIDLEPART                15338
#define CM_DELETEMIDLEPART                15339
#define CM_CURVEINVERSE                   15340
#define CM_CURVESETCLOSED                 15341
#define CM_MOVEPOINTALONGLINE             15342
#define CM_SETSURFACEDEGREE               15343
#define CM_SETCURVEDEGREE                 15344
#define CM_EDITINTERSECT                  15345
#define CM_EDITSELF_INTERSECT             15346
#define CM_POINTINTERSECT                 15347
#define CM_POINTWEIGHTCENTRE              15348
#define CM_POINTONPARAMCURVE              15349
#define CM_N_POINTS_ON_CURVE              15350
#define CM_POINTON_CURVE                  15351
#define CM_CONTOURSUBSTR                  15352
#define CM_CONTOURCROSS                   15353
#define CM_CONTOURUNITE                   15354
#define CM_CONTOURBREAK                   15355
#define CM_CONTOURCHAMFER                 15356
#define CM_CONTOURFILLET                  15357
#define CM_CONTOUREQUID                   15358
#define CM_HANDENVELOPE                   15359
#define CM_NEWENVELOPCONTOUR              15360
#define CM_NEWAPPROXCURVE                 15361
#define CM_NEWBEZIER                      15362
#define CM_NEWCBSPLN2D                    15363
#define CM_NEWHERMIT                      15364
#define CM_NEWFITSPLINE                   15365
#define CM_NEWBSPLINE                     15366
#define CM_NEWCONIC                       15367
#define CM_BUILD_BENDS_3D_AXIS_LINES      15368
#define CM_UNBENDALL                      15369
#define CM_ELLIPSE_TANGENT2CURVES         15370
#define CM_ELLIPSECENTRE3POINTS           15371
#define CM_ELLIPSE3POINTS                 15372
#define CM_ELLIPSECENTRE_AB               15373
#define CM_ARCTANGENTCURVERADPOINTON      15374
#define CM_ARCTANGENTCURVE2POINTS         15375
#define CM_ARCCIRCLE2POINTS               15376
#define CM_ARCCENTRE2POINTS               15377
#define CM_ARC2POINTSRADIUS               15378
#define CM_NEWARC_3_POINTS                15379
#define CM_CIRCLETANTHREEENTITIES         15380
#define CM_CIRCLETANTWOENTITIESRADIUS     15381
#define CM_CIRCLETANTWOENTITIESPOINTON    15382
#define CM_CIRCLETANENTITYRADIUSPOINTON   15383
#define CM_CIRCLETANENTITYTWOPOINTS       15384
#define CM_CIRCLETANENTITYCENTRE          15385
#define CM_CIRCLECENTREONTWOPOINTS        15386
#define CM_CIRCLECENTREONRADIUSPOINTON    15387
#define CM_CIRCLERADIUSTWOPOINTS          15388
#define CM_CIRCLETHREEPOINTS              15389
#define CM_CIRCLECENTRERADIUS             15390
#define CM_CIRCLECENTREPOINTON            15391
#define CM_NORMALIZE_SHEET_HOLES          15392

#define CM_VERTICALLINE                   15393
#define CM_GORISONTALLINE                 15394
#define CM_LINEBISECTOR                   15395
#define CM_LINEANGLETAN_CIRCLE            15396
#define CM_LINETAN2CIR                    15397
#define CM_LINEPNTTANG                    15398
#define CM_LINEPRPPNT                     15399
#define CM_LINEPRLPNT                     15400
#define CM_LINEPNTDIST                    15401
#define CM_LINEPOINTANGLE                 15402
#define CM_LINEPNTANLGE0X                 15403
#define CM_LINEPNTPNT                     15404

#define CM_NEW_SOLID_CONTOUR_SECTION_MAP  15406 // "Вид сеч. тв.тела контуром"
#define CM_NEW_SOLID_LOCAL_SECTION_MAP    15407 // "Местный разрез"

#define CM_NEWSOLIDHIDEMAP                15409
#define CM_NEWSURFACEHIDEMAP              15410
#define CM_NEWCURVEHIDEMAP                15411
#define CM_NEWOUTLINESOLIDS               15412

#define CM_VIEWISOPROJ                    15420
#define CM_VIEWRIGHTPROJ                  15421
#define CM_VIEWLEFTPROJ                   15422
#define CM_VIEWDOWNPROJ                   15423
#define CM_VIEWUPPROJ                     15424
#define CM_VIEWREARPROJ                   15425
#define CM_VIEWFRONTPROJ                  15426
#define CM_MAKE_ALL_REGIONS               15427 // Разбиение произвольного набора контуров на связные группы контуров
#define CM_REGIONSBOOLEANOPER             15428 // Булева операция над регионами
#define CM_MAKE_UNION_REGIONS             15429 // Объединение регионов (контуров)

#define CM_NEWPOINTWIREFRAMEINTERSECT     15434
#define CM_POINTPOSITION                  15435
#define CM_NEWNEARPOINTONSURFACE          15436
#define CM_NEWDIRECTPOINTONSURFACE        15437
#define CM_NEWPOINTONSURFACE              15438
#define CM_NEWPOINTONCURVE                15439
#define CM_NEWSURFACEISOCLINAL            15440
#define CM_NEWEVENLENGTH                  15441
#define CM_NEWEVENPARAM                   15442
#define CM_NEWCURVEISOCLINAL              15443
#define CM_NEWPOINTCURVESOLID             15444
#define CM_NEWPOINTCURVESURFACE           15445
#define CM_NEWPOINTCURVECROSSING          15446
#define CM_NEWDIRECTPOINTONSOLID          15447
#define CM_NEWPOINTONSOLID                15448
#define CM_NEWPOINT                       15449
#define CM_NEWCURVEBYPLANE                15450
#define CM_NEWPLANEOFFSET                 15451
#define CM_RECTANGLEONPLANE               15452
#define CM_NEWCONTOURONPLANE              15453
#define CM_NEWPLANENURBS                  15454
#define CM_NEWPLANECBSPLN                 15455
#define CM_SHELLSEW                       15456
#define CM_SOLIDSEW                       15457
#define CM_SHELLSURFACE                   15458
#define CM_NEWLINEPLANEPLANE              15459
#define CM_NEWLINE3D                      15460
#define CM_NEWLINEPOINTPERPCURVE          15461
#define CM_NEWLINEPOINTPERPSOLID          15462
#define CM_NEWLINEPOINTPERPSURFACE        15463
#define CM_NEWSOLIDINTERSECTION           15464
#define CM_NEWSURFACESOLIDINTERSECT       15465

#define CM_NEWSURFACEINTERSECTION         15467
#define CM_NEWMESH_BYFACE                 15468
#define CM_NEWEXTENDEDCURVE3D             15469
#define CM_NEWTRIMMEDCURVE3D              15470
#define CM_NEWOFFSETCURVE3D               15471
#define CM_NEWSURFACEINTERSECTIONONEPOINT 15472
#define CM_NEWCURVEDIRECTVECTOR           15473
#define CM_NEWCURVEONSURFACE              15474
#define CM_NEWSURFACEBORDER               15475
#define CM_NEWSPACECURVEBYPLANE           15476
#define CM_SWEPTSECTIONSURFACE            15477
#define CM_NEWCHARCURVE3D                 15478
#define CM_NEWNURBS3D                     15479
#define CM_NEWBEZIER3D                    15480
#define CM_NEWHERMIT3D                    15481
#define CM_NEWSPIRALCURVE                 15482
#define CM_NEWARC3D                       15483
#define CM_NEWPOLYLINE3D                  15484
#define CM_NEWLINESEGMENT3D               15485
#define CM_NEWEVOLUTIONSECTION            15486 // кривая, трансформированная по закону кинематической операции

#define CM_NEWELLIPSE3D                   15488
#define CM_NEWCIRCLE3D                    15489
#define CM_SOLIDFILLET                    15490
#define CM_SOLIDCHAMFER                   15491
#define CM_FULLFILLETSOLID                15492
#define CM_NEWCURVESCOUPLE3D              15493
#define CM_NEWSYMMETRYSOLID               15494
#define CM_SOLID_CONTOUR_CUT              15495
#define CM_SOLID_SURFACE_CUT              15496
#define CM_BOOLEANINTERSECTION            15497
#define CM_BOOLEANDIFFERENCE              15498
#define CM_BOOLEANUNION                   15499
#define CM_NEWSOLIDBYSURFACE              15500
#define CM_NEWSHEETSOLID                  15501
#define CM_NEWPYRAMIDSOLID                15502
#define CM_NEWPRISMSOLID                  15503
#define CM_SILHOUETTECONTOURSOLID         15504
#define CM_NEWRULEDSOLID                  15505
#define CM_LATHESECTIONCURVES             15506
#define CM_NEWTETRAELEMENT                15507
#define CM_NEWOCTAELEMENT                 15508
#define CM_NEWCURVESFILLET3D              15509
#define CM_NEWTORUSSOLID                  15510
#define CM_NEWSPHERESOLID                 15511
#define CM_NEWCONESOLID                   15512
#define CM_NEWCYLINDERSOLID               15513
#define CM_NEWWEDGESOLID                  15514
#define CM_NEWBLOCKSOLID                  15515
#define CM_NEWPLATESOLID                  15516
#define CM_NEWCURVEBOUNDEDSURFACE         15517
#define CM_TRUNCATECURVES                 15518
#define CM_UNBENDPLATE                    15519
#define CM_NEWJOINSURFACE                 15520
#define CM_NEWCURVESCONNCURVE3D           15521 // Создание соединяющей кривой
#define CM_NEWEXTENDEDSURFACE             15522
#define CM_NEWGRIDSURFACE                 15523
#define CM_NEWOFFSETSURFACE               15524
#define CM_NEWMESHSURFACE                 15525 // Создание сетчатой поверхности
#define CM_NEWLOFTEDSURFACE               15526
#define CM_NEWNURBSSURFACE                15527
#define CM_NEWCOVERSURFACE                15528
#define CM_NEWCORNERSURFACE               15529
#define CM_NEWDEFORMEDSURFACE             15530 // Создание деформированной поверхности
#define CM_NEWSPLINESURFACE               15531
#define CM_DUPLICATION_BYGRID             15532
#define CM_DUPLICATION_BYMATRICES         15533
#define CM_MEDIAN_SHELL                   15534
#define CM_LATTICE_SHELL                  15535
#define CM_DIVIDED_SHELL                  15536
#define CM_DOUBLE_MESH                    15537
#define CM_ITEMSMERGING                   15538
#define CM_SETCONNECTIONCOLOR             15539
#define CM_SETCOLORSELECTEDCONN           15540
#define CM_SETSTYLEWIDTH                  15541
#define CM_SETOBJECTWIDTH                 15542
#define CM_SETCURSORSTEP                  15543
#define CM_SETMESH                        15544
#define CM_SETSELECTCOLOR                 15545
#define CM_SETOBJECTCOLOR                 15546
#define CM_SETCOLORSELECTED               15547
#define CM_DELETEINVISIBLE                15548
#define CM_SMARTCURSOR                    15549
#define CM_COONSPATCHSURFACE              15550
#define CM_SETDEVIATIONSAG                15551
#define CM_NEWNEARPOINTONCURVE            15552
#define CM_SETMAKEGRID                    15553
#define CM_SET_OGL_VIEWTYPE               15554
#define CM_SETVISUALSAG                   15555
#define CM_WINDOWCLOSE_ALL                15556
#define CM_WINDOWCLOSE_ACTIVE             15557
#define CM_CHANGESCALE                    15558
#define CM_SETDEPTH                       15559
#define CM_SETPROJECTION                  15560
#define CM_WINDOWARRANGE                  15561
#define CM_WINDOWCASCADE                  15562
#define CM_WINDOWTILE                     15563
#define CM_VIEWISO                        15564
#define CM_VIEWRIGHT                      15565
#define CM_VIEWLEFT                       15566
#define CM_VIEWDOWN                       15567
#define CM_VIEWUP                         15568
#define CM_VIEWREAR                       15569
#define CM_VIEWFRONT                      15570
#define CM_FILESAVE                       15571
#define CM_FILEADD                        15572
#define CM_FILENEW                        15573
#define CM_FILEOPEN                       15574
#define CM_GETCURVERADIUS                 15575
#define CM_GETCURVELENGTH                 15576
#define CM_GETDISTANCETOPOINT             15577
#define CM_TESTEXIT                       15578
#define CM_DELETESELECTED                 15579
#define CM_HELPABOUT                      15580
#define CM_DELETEALLPLANEOBJECT           15581
#define CM_NORMALIZEOBJECT                15582
#define CM_SOLIDVERTEXFILLET              15583
#define CM_FITSURFACE                     15584
#define CM_FACEFILLETTYPE                 15585
#define CM_FILLETRADIUSSET                15586
#define CM_FITSURFACE_AXIS_CONSTRAINT     15587
#define CM_GETWIREFRAMELENGTH             15588
#define CM_FACESMERGINGSOLID              15591
#define CM_NEWCURVECURVATUREPOINT         15592
#define CM_SPLINEPATCHSURFACE             15593
#define CM_GETEDGENUMBER_INFACE           15594 // Номер ребра в грани с указанием номера цикла
#define CM_GETFACENUMBER                  15595
#define CM_GETEDGENUMBER                  15596    
#define CM_GETFACEBYNUMBER                15597
#define CM_GETEDGEBYNUMBER                15598
#define CM_CLOSECORNER                    15599
#define CM_SHELLPATCH                     15600 
#define CM_CURVEPATCH                     15601
#define CM_BREAKSOLIDTOPARTS              15602 
#define CM_SOLIDVARIETY                   15603 
#define CM_SOLIDEXTERNAL                  15604 
#define CM_SOLIDINTERNAL                  15605
#define CM_NEWSIMPLEXSPLINESURFACE        15606
#define CM_NEWCURVESBRIDGE3D              15607
#define CM_JOGSHEETSOLID                  15608
#define CM_SHEETSOLIDINTERSECT            15609
#define CM_NEWELEVATIONSURFACE            15610 
#define CM_FILLETCURVES                   15611
#define CM_CHAMFERCURVES                  15612
#define CM_STAMPSHEETSOLID                15613
#define CM_SIMPLIFY_TRGN_CONVEX_HULL      15614
#define CM_ADDNURBSTONURBS                15615
#define CM_CREATESHEETRIB                 15616
#define CM_BEADSHEETSOLID                 15617
#define CM_JALOUSIESHEETSOLID             15618
#define CM_NEWEDGEMAP                     15619 
#define CM_NEWEDGEMAPONPLANE              15620 
#define CM_NEWNURBSBLOCKSOLID             15621
#define CM_NEWSOLIDSHIDEMAP               15622
#define CM_NEWHOLESOLID                   15623  
#define CM_TRIANGULATESOLIDS              15624
#define CM_JOINTBEND                      15625
#define CM_SOLIDSLANT1                    15626
#define CM_SOLIDSLANT2                    15627
#define CM_SOLIDSPAN                      15628
#define CM_CREATERULEDSOLID               15629
#define CM_POINTDISTANCETOPOINT           15630
#define CM_NEWPOCKETSOLID                 15631
#define CM_NEWELEMENTARYSOLID             15632
#define CM_REBUILDTIMERESULTFILE          15633
#define CM_NEWSLOTSOLID                   15634
#define CM_NEWINSTANCE                    15635
#define CM_NEWASSEMBLY                    15636
#define CM_DECOMPOSE_ALL_ASSEMBLIES       15637 // Разрушить сборки с подсборками на составляющие
#define CM_NEWMESH_BYSHELL                15638 
#define CM_NEWCONVEXPOLYHEDRON            15639
#define CM_REMOVEMODIFIEDSOLID            15640
#define CM_CREATEMODIFIEDSOLID            15641
#define CM_ACTIONMODIFIEDSOLID            15642
#define CM_OFFSETMODIFIEDSOLID            15643
#define CM_FILLETMODIFIEDSOLID            15644
#define CM_SUPPLEMODIFIEDSOLID            15645
#define CM_NEWMESHSHELL                   15646 // Создание оболочки по сетке кривых
#define CM_NEWRULSHELL                    15647 // Создание линейчатой оболочки
#define CM_ATTACHNURBSTOOBJECT            15648 // Состыковать сплайн с объектом 
#define CM_NEWEXTENSIONSHELL              15649
// BEG Резьба
#define CM_NEWTHREAD_INSPACE              15650
#define CM_NEWTHREAD_ONSOLID              15651
#define CM_NEWTHREAD_MAPPING_BYPARAMS     15652
#define CM_NEWTHREAD_MAPPING_BYTHREAD     15653
#define CM_MODTHREAD_ISMATEDTHREADS       15654
#define CM_MODTHREAD_ISBODYTHREAD         15655
#define CM_MODTHREAD_ADAPTTOBODY          15656
// END Резьба
// BEG Габарит объекта относительно л.с.к.
#define CM_LOCALGABARIT_CURVE2D           15660
#define CM_LOCALGABARIT_CURVE3D           15661
#define CM_LOCALGABARIT_SURFACE           15662
#define CM_LOCALGABARIT_EDGE              15663
#define CM_LOCALGABARIT_FACE              15664
#define CM_LOCALGABARIT_SOLID             15665
// END Габарит объекта относительно л.с.к.

#define CM_FLOOD_FILL_VOLUME              15667
#define CM_CONVERT_TO_MESH                15668
// BEG Тонкостенное тело
#define CM_OFFSET_SOLID                   15669
#define CM_NEWTHINSOLID_ONESIDE_OFFSET    15670
#define CM_NEWTHINSOLID_ONESIDE_MONOTHICK 15671
#define CM_NEWTHINSOLID_ONESIDE_POLYTHICK 15672
#define CM_NEWTHINSOLID_TWOSIDE_MONOTHICK 15673
#define CM_NEWTHINSOLID_TWOSIDE_POLYTHICK 15674
#define CM_THINSOLIDBYSHELL               15678
// END Тонкостенное тело (занято до 15679 включительно)

#define CM_RADIUSDIMENSION                15680
#define CM_SURF_NORMALS                   15681
#define CM_FACE_NORMALS                   15682
#define CM_CREATE_PARTSOLID               15683
#define CM_TRANSFORMEDSOLID               15684
#define CM_MESH_NORMALS                   15685

#define CM_NEWCONTOUR3DBYCURVES           15686

#define CM_SURFAXESDISTANCE               15689 // Расстояние между осями поверхностей, имеющих оси
#define CM_GEOMITEMS_MAXDISTANCE          15690 // Максимальное расстояние между геометрическими объектами
#define CM_TOPOITEMS_MAXDISTANCE          15691 // Максимальное расстояние между топологическими объектами

// BEG Мультилиния
#define CM_CREATECNTRSMULTILINE           15692
#define CM_CREATEMULTILINE                15693
#define CM_REPLACEMLTLINESBASISCURVE      15694
#define CM_INSERTMLTLINESVERTEX           15695
#define CM_REMOVEMLTLINESVERTEX           15696
#define CM_SETMLTLINESHOTPOINT            15697
#define CM_DELMLTLINESHOTPOINT            15698
#define CM_SETMLTLINESVERTSMOOTHJOINT     15699
#define CM_SETMLTLINESVERTTRACINGTYPE     15700
#define CM_SETMLTLINESVERTSPECFILLETRAD   15701
#define CM_SETMLTLINESVERTTIPTYPE         15702
#define CM_SETMLTLINESVERTTIPDIR          15703
#define CM_SETMLTLINESRADIUS              15704
#define CM_CHANGEMLTLINESRADIUS           15705
#define CM_ADDMLTLINESRADIUS              15706
#define CM_REMOVEMLTLINESRADIUS           15707
#define CM_SETMLTLINESBEGTIPTYPE          15708
#define CM_SETMLTLINESBEGTIPPARAM         15709
#define CM_SETMLTLINESENDTIPTYPE          15710
#define CM_SETMLTLINESENDTIPPARAM         15711
#define CM_SETMLTLINESPROCESSCLOSED       15712
#define CM_FILLETMLTLINESBASISCURVE       15713
#define CM_CHAMFERMLTLINESBASISCURVE      15714
#define CM_BREAKMLTLINE                   15715
#define CM_BREAKMLTLINENPARTS             15716
#define CM_DELMLTLINESLASTSEG             15717
#define CM_DELETEPARTMLTLINEP1P2          15718
#define CM_NEWFILLETCONTOUR3DBYCURVES     15719
#define CM_NEWCHAMFERCONTOUR3DBYCURVES    15720
// END Мультилиния (занято до 15720 включительно)

#define CM_GRIDDED_SURF_MINMAXDISTANCES   15721 // Расстояния между поверхностями по сетке
#define CM_TRIANGULAREREGION              15722
#define CM_CHECKIDENTICALBASESURFACES     15723 // Проверить и устранить наличие общих подложек
#define CM_UNIFYOWNCOMPLANARFACES         15724 // Объединить компланарные грани тела
#define CM_CHECK_CURVE2D_SPEED            15725 // Проверка скорости работы двумерной кривой
#define CM_CHECK_CURVE3D_SPEED            15726 // Проверка скорости работы трехмерной кривой
#define CM_CHECK_SURFACE_SPEED            15727 // Проверка скорости работы поверхности
#define CM_CHECK_SOLIDS_INTERSECT         15728 // Предпроверка тел на пересечение
#define CM_CUTSHELL_BYOBJECTS             15729 // Усечение оболочки выбранными объектами
#define CM_NEWPOINTS_ON_SURFACE           15730 // Группа точек на поверхности
#define CM_SHELL_REVERSE                  15731 // Вывернуть оболочку
#define CM_NEWCURVE_BY_TWO_PROJECTIONS    15732 // Создать кривую по двум плоским проекциям
#define CM_NEWCURVE_ON_SURFACE_BY_POINTS  15733 // Создать кривую на поверхности по точкам
#define CM_NEWPROJECTIONCURVE_ON_SURFACE  15734 // Создать проекционную кривую на поверхности
#define CM_GEOMITEMS_MINDISTANCE          15735 // Мин. расстояние между геометрическими объектами
#define CM_CREATE_SURFACE_OPER_TESS       15736 // Создать операционную разбивку поверхности
#define CM_SURF_MAX_MIN_CURVATURES        15737 // Максимальная и минимальная кривизну поверхности  
#define CM_FACES_MAX_MIN_CURVATURES       15738 // Максимальная и минимальная кривизну граней 
#define CM_NEWCONTOURWITHBREAKS           15739 // Создать контур 
#define CM_ADDINVISIBLEPART               15740 // Добавить в контур невидимую часть
#define CM_ADD_ATTTRIBUTE                 15741 // Добавить атрибут телу или его примитиву
#define CM_NEWJOINSOLID                   15742 // Создать оболочку на основе поверхности соединения
#define CM_CURVECURVATURE                 15743 // Построить график кривизны кривой
#define CM_CURVETORSION                   15744 // Построить график кручения кривой
#define CM_NEWNURBS3D_WITH_MATING         15745 // Построить сплайн по точкам и производным
#define CM_CURVEADDKNOTE                  15746 // Добавить узел в NURBS кривую
#define CM_NEWCURVESWRAPPING              15747 // Обертывание кривыми поверхности // Surface curves wrapping
#define CM_CHECK_CURVE2D_SELFINT          15748 // Проверить двумерную кривую на самопересечение
#define CM_CURVEREMOVEKNOTE               15749 // Удалить NURBS узел
#define CM_SET_SOLID_LAST_VERSION         15750 // Выставить последнюю версию
#define CM_NEWPLANEEDGE                   15751 // Создать плоскость по ребру параллельно оси координат
#define CM_GETPLANECURVE                  15752 // Создать кривую по плоской пространственной
//#define CM_NEW_CONSTRUCTIVE_BY_POINTS     15753 // Создать вспомогательный объект по трем точкам
#define CM_CHECK_CURVES_TANGENT_MATING    15754 // Проверить касание кривых
#define CM_NEWNURBSCONIC3D_1              15755 // Создать коническое сечение по 2-ум точкам вершине и дискриминанту
#define CM_NEWNURBSCONIC3D_2              15756 // Создать коническое сечение по 3-ем точкам и вершине
#define CM_NEWNURBSCONIC3D_3              15757 // Создать коническое сечение по 3-ем точкам и 2-ум наклонам
#define CM_NEWNURBSCONIC3D_4              15758 // Создать коническое сечение по 2-ум точкам, 2-ум наклонам и дискриминанту
#define CM_NEWNURBSCONIC3D_5              15759 // Создать коническое сечение по 4-ем точкам и наклону
#define CM_NEWNURBSCONIC3D_6              15760 // Создать коническое сечение по 5-ти точкам
#define CM_SUPPLESETCONJUGATION           15761 // Модификация тела c измененными гранями установкой сопряжений по ребрам
#define CM_SUPPLESETSIMILARITY            15762 // Модификация тела c измененными гранями подобием указанной поверхности
#define CM_SUPPLESETSMOOTHING             15763 // Модификация тела c измененными гранями сглаживанием
#define CM_APPROXBYARCS                   15764 // Аппроксимировать кривую дугами и отрезками
#define CM_SHOW_CURVE_DIRECTION           15765 // Показать направление кривой
#define CM_PURIFYMODIFIEDSOLID            15766 // Модификация тела удалением грани скругления
#define CM_FIND_OVERLAPPING_SIMILAR_FACES 15767 // Найти подобные грани с наложением
#define CM_FIND_FACES_EDGES_CARRIERS      15768 // Найти грани - потенциальные носители ребер
#define CM_SETVIEWVECTOR                  15769 // Расположить по вектору взгляда.
#define CM_CHECK_CONVEX_HULL_INTERSECT    15770 // Проверка абрис триангуляций тел на пересечение
#define CM_DELETE_ITEM_CREATORS           15771 // Удалить историю построения
#define CM_SKIP_ASSERTS                   15772 // Пропускать ассерты
#define CM_EDIT_SAME_CREATORS             15773 // Не копировать строители операций при редактировании их параметров (свойств)
#define CM_SWITCH_MULTITHREADING          15774 // Переключить режим многопоточности
#define CM_UNWRAPCURVE                    15775 // Развертка кривой
#define CM_POINTLOCATION_RELATIVETOBODY   15776 // Положение точки относительно тела
#define CM_NEWALLSOLIDSHIDEMAP            15777 // Проекция всех тел
#define CM_REMOVE_ATTTRIBUTES             15778 // Удалить все атрибуты тела
#define CM_MESH_TO_BREP                   15779 // Преобразовать полигональный объект в тело
#define CM_NEW_ICOSAHEDRON_MESH           15780 // Создать икосаэдр (полигональный объект)
#define CM_NEW_ICOSAHEDRON_SOLID          15781 // Создать икосаэдр (тело)
#define CM_NEW_SPHERE_POLYHEDRON_MESH     15782 // Создать аппроксимацию сферы многогранником (полигональный объект)
#define CM_NEW_SPHERE_POLYHEDRON_SOLID    15783 // Создать аппроксимацию сферы многогранником (тело)
#define CM_NEWCOSINUSOID_BY_3_POINTS      15784 // Создать косинусоиду по трем точкам и параметрам 
#define CM_NEWCOSINUSOID_BY_ORIGIN        15785 // Создать косинусоиду по началу и параметрами
#define CM_CHECK_CURVE_POLYGON_ACCURACY   15786 // Проверить точность полигона кривой
#define CM_ELIMINATE_CONTOUR_GAPS         15787 // Устранить разрывы в контуре
#define CM_MULTITHREADED_MODE             15788 // Multithreaded mode
#define CM_LIST_OF_MODES                  15789 // Список режимов многопоточности (List of multithreaded modes)
#define CM_SIMPLIFY_MESH                  15790 // Упростить полигональный объект (Simplify a mesh)
#define CM_CHECK_READ_TIME                15791 // Выводить время чтения модели (Show read time of model)
#define CM_CHECK_WRITE_TIME               15792 // Выводить время записи модели (Show write time of model)
#define CM_LEAVE_LAST_ITEM_CREATORS_ONLY  15793 // Оставить только последние элементы истории построения
#define CM_WRITE_MODEL_TREE               15794 // Запись дерева модели
#define CM_TREEOPEN                       15795 // Открыть дерево модели
#define CM_TREE_TYPE                      15796 // Прочитать объекты по типу
#define CM_TREE_GABARIT                   15797 // Прочитать объекты по габариту
#define CM_WINDOWCLOSE_EMPTY              15798 // Close empty windows
#define CM_RESET_SURFACE_BBOX             15799 // Reset surface bounding box
#define CM_CHECK_FACE_TRIANGULATION       15800 // Проверить триангуляцию грани
#define CM_FASTENERBYPOINT                15801 // Создать крепеж по точке
#define CM_FASTENERBYCURVE                15802 // Создать крепеж по кривой
#define CM_TREE_ATTRIBUTE                 15803 // Прочитать объекты по атрибуту
#define CM_SHOW_SURFACE_DIRECTIONS        15804 // Показать направления поверхности (Show surface directions)
#define CM_CONSOLE                        15805 // Запустить консоль (Run the console)
#define CM_TREE_EMBODIMENT                15806 // Прочитать дерево исполнений
#define CM_GETHERMODIFIEDSOLID            15807

#define CM_CHANGE_SHELL_CLOSEDNESS              15808 // Save shell's closedness
#define CM_REMOVE_INNER_LOOP                    15809 // Remove internal loop
#define CM_NEWCURVE_CENTER_LINE                 15810 // Создать осевые линии для грани оболочки
#define CM_CHECK_SHELLS_PTRS_COINCIDENCE        15811 // Проверить совпадение указателей в оболочках
#define CM_CURVATUREPROFILE                     15812 // Построить график кривизны кривой
#define CM_SOLID_SHELL_CUT                      15813 // Cut solid by a shell
#define CM_CUTSHELL_BYSHELL                     15814 // Cut unclosed shell by a shell
#define CM_FILESAVE_PREV_RELEASE                15815 // Save model to previous release version
#define CM_FILESAVE_LAST_RELEASE                15816 // Save model to last release version
#define CM_SOLIDS_MINDISTANCE                   15817 // Минимальное расстояние между телами
#define CM_MERGERMODIFIEDSOLID                  15818 // Direct modeling Удаление указанных рёбер тела. 
#define CM_REPAIR_INEXACT_EDGES                 15819 // Try to repair inexact edges
#define CM_USERSTAMPSHEETSOLID                  15820 // Sheet solid stamping by tool solid.
#define CM_CURVE_MAX_MIN_CURVATURES             15821 // Максимальная и минимальная кривизну кривой
#define CM_SOLID_REPAIRING                      15822 // Починить тело. // Solid repairing.
#define CM_WORK_OF_CREATORS                     15823 // Включение/отключение работы построителей. // Enable/disable the work of constructors.
#define CM_NEWPOINTS_BY_SOLID                   15824 // Точечный каркас по вершинам тела // Point frame by solid vertices
#define CM_MERGE_POINTS                         15825 // Объединить точечные каркасы // Create point frames union
#define CM_CHECK_POINTFRAME_NAMES               15826 // Проверить имена на совпадение. // Check point frame names for a match.
#define CM_CHECK_WIREFRAME_NAMES                15827 // Проверить имена на совпадение. // Check wire frame names for a match.
#define CM_RESET_INTCURVE_APPROX                15828 // Reset approximation of surfaces intersection curve
#define CM_PERFORM_SHELL_NAMING                 15829 // Perform shell naming
#define CM_MESH_TO_GRIDS                        15830 // Разделить триангуляции.    // Split on grids.
#define CM_KEEP_ONLY_GRIDS                      15831 // Оставить только пластины. // Keep only grids.
#define CM_KEEP_ONLY_POLYS                      15832 // Оставить только ломаные.  // Keep only polys.
#define CM_KEEP_ONLY_APEXS                      15833 // Оставить только апексы.   // Keep only apexs.
#define CM_SECTION_SHELL                        15834 // Построить оболочку переменного сечения. // Mutable section shell construction.
#define CM_NEWCURVE_BY_VERTICES                 15835
#define CM_SHEETMETAL_BYSOLID                   15836 // Построить оболочку листового тела по произвольному телу. // Build the sheet metal solid based on an arbitrary solid.
#define CM_NEWLENGTHCURVE                       15837 // Создать сплайн между заданными точками заданной длины. Create a spline between specified points of the specified length.
#define CM_NEWFACECURVE                         15838 // Создать сплайн на поверхности грани между между заданными точками. Create a spline on the surface of the face between between the specified points.
#define CM_CHECK_SKETCH                         15839 // Проверить эскиз. // Create sketch.
#define CM_TRANSLATE_SPACECURVE_TO_SURFACECURVE 15840 // Translate space curve into curve-on-surface form.
#define CM_CHECK_ALL_SHELLS_GEOMETRY            15841 // Проверить геометрию всех оболочек. // Check geometry of all shells.
#define CM_REDUCE_NURBS_DEGREE                  15842 // Уменьшить порядок nurbs кривой на 1. // Reduce the order of the nurbs curve at 1.
#define CM_RAISE_NURBS_DEGREE                   15843 // Увеличить порядок nurbs кривой на 1. // Raise the order of the nurbs curve at 1.
#define CM_CURVE_FACEPROJECTION                 15844 // Создать проекцию кривой на поверхности грани. // Create a curve projection on the surface of the face.
#define CM_DERIVATIVEPROFILE                    15845 // Построение эпюр производных поверхности по кривой на этой поверхности. // The surface derivative plotting by the curve on this surface.
#define CM_REPAIRINCONSISTENTMESH               15846 // Исправить несогласованную ориентацию сетки. // Repair inconsistent orientation.
#define CM_CURVEXTEND                           15847 // Удлинить пространственную или плоскую кривую. // Extend a planar or a spatial curve.
#define CM_SWITCH_DEBUG_DRAW                    15848 // Отключить/включить отладочную отрисовку. // Disable/enable debug drawing.
#define CM_FITMODELTOWINDOW                     15849 // Вписать модель по размеру окна. // Fit model to window. 

#define CM_MESH_REVERSE                         15850 // Вывернуть полигональный объект. // Reverse a polygonal object.
#define CM_TRANSLATEMESHTOBASIS                 15851 // Один шаг назад в построении полигонального объекта. // Step back in building of a polygonal object.
#define CM_REBUILD_MESH                         15852 // Перестроить полигональный объект. // Rebuild a polygonal object.
#define CM_CONVERT_MESH_TO_INSTANCE             15853 // Заменить повторяющиеся меши на вставки. // Convert repetitive meshes to instances.
#define CM_CHECK_SHELL_MAIN_NAMES               15854 // Проверить главные имена рёбер и вершин. // Check the main names of edges and vertices.
#define CM_RESET_CURVES_TEMPDATA                15855 // Сбросить временные данные у кривых. // Reset temporary data of curves.
#define CM_POLYLINE_BY_SELECTED                 15856 // Создание ломаной линии. // Creating a polyline.  

#define CM_NEWCURVESUNWRAPPING                  15857 // Развёрка кривой с поверхности. // Unwrap curve from a surface.
#define CM_FIT_CURVE_TOLERANCE                  15858 // Аппроксимация гладкой нурбс кривой по точности. // Fit smooth nurbs by tolerance.
#define CM_FIT_CURVE_UNIFORM                    15859 // Аппроксимация гладкой нурбс кривой с равномерным узловым вектором. // Fit smooth nurbs with uniform knots.

#define CM_NEWPLANE_BYCURVE                     15860 // New plane by planar curve
#define CM_CHECK_OPERATION_TIME                 15861 // Выводить время операции (Show operation time)
#define CM_SIMPLIFY_MESH_TRI_COUNT              15862 // Упростить триангуляцию до заданного количества треугольников. // Simplify mesh up to specified triangles count.
#define CM_SIMPLIFY_MESH_TOLERANCE              15863 // Упростить триангуляцию с заданной точностью. // Simplify mesh with specified tolerance.
#define CM_NEW_MESH_CONVEX_HULL                 15864 // Создать выпуклую оболочку для триангуляции. // Create convex hull for mesh.
#define CM_CHECK_CURVE3D_SELFINT                15865 // Проверить трехмерную кривую на самопересечение // Check 3D curve for self-intersection

#define CM_ROTATEMODIFIEDSOLID                  15866 // Прямое моделирование: поворот граней. // Direct modeling: Rotate faces

#define CM_NEWWIREFRAMEEXTRUSIONSOLID           15867 // Построение тела выдавливания проволочного каркаса. // Building of the extrusion solid of the wire frame.
#define CM_NEWWIREFRAMEREVOLUTIONSOLID          15868 // Построение тела вращения проволочного каркаса. // Building of the revolution solid of the wire frame.
#define CM_NEWWIREFRAMEEVOLUTIONSOLID           15869 // Построение тела движения проволочного каркаса. // Building of the evolution solid of the wire frame.
#define CM_NEWWIREFRAMEEXTRUSIONSHELL           15870 // Построение оболочки выдавливания проволочного каркаса. // Building of the extrusion shell of the wire frame.
#define CM_NEWWIREFRAMEREVOLUTIONSHELL          15871 // Построение оболочки вращения проволочного каркаса. // Building of the revolution shell of the wire frame.
#define CM_NEWWIREFRAMEEVOLUTIONSHELL           15872 // Построение оболочки движения проволочного каркаса. // Building of the evolution shell of the wire frame.
#define CM_NEW_WIREFRAME_BY_FACE                15873 // Создать проволочный каркас по грани тела. // Create wire frame by face of solid.
#define CM_FIND_NEAREST_SOLID                   15874 // Найти ближайшее тело. // Find nearest solid
#define CM_MODE_SPECIAL_HIGHLIGHT               15875 // Подсветка подобъектов в меню. // Highlight sub-objects in the menu.

#define CM_FIT_CIRCLE_LS                        15876 // Аппроксимация окружностью методом наименьших квадратов. // Fit circle by least squares method.
#define CM_FIT_LINE_LS                          15877 // Аппроксимация прямой методом наименьших квадратов. // Fit line by least squares method.
#define CM_FIT_ELLIPSE_LS                       15878 // Аппроксимация эллипса методом наименьших квадратов. // Fit ellipse by least squares method.

#define CM_ALIGN_MESH                           15879 // Совместить триангуляцию с другим объектом. // Align a mesh with another object.

#define CM_REPLACEMODIFIEDSOLID                 15880 // Прямое моделирование: замена граней. // Direct modeling: replacement of faces.
#define CM_VOXEL_REPRESENTATION                 15881 // Построение воксельного представления. Voxel representation of the model.
#define CM_REPAIR_VERTICES                      15882 // Try to repair vertices
#define CM_REMOVE_COLOR_AND_VISUAL_ATTRIBUTES   15883 // Удаление всех визуальных атрибутов. // Removing all visual attributes.

#define CM_NEW_NURBS_WIREFRAME                  15884 // Создание каркаса NURNS кривых. // Creating a wire frame of NURNS curves.
#define CM_NEW_TRIMMED_WIREFRAME                15885 // Создание усеченного каркаса. // Creating a trimmed wire frame.

#define CM_UNWRAPPOINTS                         15886 // Развертка точек.  // Unwrap points.
#define CM_WRAPPOINTS                           15887 // Свертка точек.  // Wrap points.
#define CM_FITONFIRSTLOADING                    15888 // Включить адаптацию модели в окно. // Enable model adaptation in the window.
#define CM_DRAP_SHELL                           15889 // Драпировка оболочки. // Drap shell.
#define CM_NEW_EXTENDED_WIREFRAME               15890 // Создание продленного каркаса. // Creating an extended wire frame.

#define CM_SECTION_SHELL_CUT                    15891 // Построить оболочку переменного сечения с обрезкой. // Mutable section shell construction with cutting.
#define CM_CHECK_SURFACE_SELFINT                15892 // Проверить поверхность на самопересечение. // Check surface for self-intersection.
#define CM_CHECK_SHELL_SELFINT                  15893 // Проверить оболочку на самопересечение. // Check shell for self-intersection.

#define CM_REPLACEFACESGROUP                    15894 // Прямое моделирование: замена группы граней. // Direct modeling: replacement group of faces.

//#define CM_WRITE_VIEW_DATA                      15895 // Запись текущего вида. // Writing the current view.

#define CM_NEW_OFFSET_WIREFRAME                 15896 // Создание эквидистантного каркаса. // Creating a offset wire frame.

#define CM_THREEFACESFILLET                     15897 // Скругление трех граней. // Three-face fillet.

#define CM_NEWCONTOURS3DBYWIREFRAME             15898 // Контуры по ребрам проволочного каркаса. // Contours along the edges of the wire frame.
#define CM_NEW_WIREFRAME_CONTOURS3D             15899 // Контуры в виде каркасов по ребрам проволочного каркаса. // Contours in the form of frames along the edges of the wire frame.
#define CM_NEW_WIREFRAME_PARTS                  15900 // Отделение частей каркаса. // Separating the frame parts.
#define CM_REMOVEINTERNALVOLUMES                15901 // Удаление внутренних объемов тела // Removal of internal body volumes
#define CM_MEDIANCURVE                          15902 // Создание срединной кривой. // Creating a median curve.

#define CM_FILLET_BY_BORDERS                    15903 // Скругление по граничным кривым. // Fillet by border curves.
#define CM_RESIZE_CYLINDER                      15904 // Прямое моделирование: изменение размера цилиндрич. граней. // Direct modeling: resize cylindric faces.
#define CM_RESIZE_SPHERE                        15905 // Прямое моделирование: изменение размера сферич. граней. // Direct modeling: resize spherical faces.

#define CM_REBUILD_WIRE_FRAME                   15906 // Перестроить проволочный каркас. // Rebuild the wire frame.
#define CM_REBUILD_ALL_WIRE_FRAMES              15907 // Перестроить все проволочные каркасы. // Rebuild all wire frames.

#define CM_MERGE_EDGES_WIREFRAME                15908 // Слияние ребер проволочного каркаса. // Merge edges of wireframe.

// MeshCheck
#define CM_SHOW_DEGENERATE_TRIANGLES      16029 // Показать вырожденные треугольники сетки ( Show the degenerate triangles of the mesh )
// MeshCheck

#define CM_EXTRACT_BEAM                   16030 // Поиск балочных элементов

#define CM_ADD_VIEW_ATTR                  16031 // Добавить (изменить) атрибуты вида  //Add (change) view attributes
#define CM_REMOVE_VIEW_ATTR               16032 // Удалить атрибуты вида //Remove view attributes

// BEG Mesh2Brep
#define CM_MESH_TO_BREP_SURF_RECON_AUTO   18810 // Преобразовать полигональный объект в тело c распознаванием поверхностей (Convert mesh to solid with surface reconstruction).
#define CM_MESH_CURVATURES                18811
#define CM_MESH_CURVATURES_DIR            18812
#define CM_MESH_CURVATURES_CLEAR          18813
#define CM_MESH_SEGMENTATION_MAP          18814
#define CM_MESH_PATH                      18815
#define CM_MESH_SEGMENTATION_SEGMENT      18816
// END Mesh2Brep

//------------------------------------------------------------------------------
// Тесты ограничений
//---
#define CM_COMMON_MATES_TEST              19000 // Общее тестирование сопряжений.
#define CM_MATE_CUBE                      19001 // Создать сборку из кирпичей, жестко соединенных  в линию, стенку или куб.
#define CM_MATE_FRACTAL                   19002 // Создать фрактальную сборку.
#define CM_MATE_DISTRIBUTED_DOF           19003 // Создать сборку с распределенными степенями свободы.
#define CM_LINEAR_PATTERN                 19004 // Создать линейный массив компонент.
#define CM_ANGULAR_PATTERN                19005 // Создать угловой массив компонент.

//------------------------------------------------------------------------------
// Видимость объектов
//---
#define CM_INVISIBLEOBJECT                20115
#define CM_VISIBLEALLOBJECTS              20116
#define CM_VISIBLEPOINTS                  20117
#define CM_VISIBLECURVES                  20118
#define CM_VISIBLESURFACES                20119
#define CM_VISIBLEBODIES                  20120
#define CM_PLACEMENT_VISIBILITY           20121 // Coordinate system visibility

//------------------------------------------------------------------------------
// Геометрические ограничения
// Geometric constraints
//---
#define CM_GC_COINCIDENCE                 20200
#define CM_GC_PARALLELISM                 20201
#define CM_GC_PERPENDICULAR               20202
#define CM_GC_TANGENCY                    20203
#define CM_GC_CONCENTRIC                  20204
#define CM_GC_DISTANCE                    20205
#define CM_GC_ANGLE                       20206

//------------------------------------------------------------------------------
// Сборочные сопряжения
// Assembly mates
//---
#define CM_MATE_COINCIDENCE               20240
#define CM_MATE_PARALLELISM               20241
#define CM_MATE_PERPENDICULAR             20242
#define CM_MATE_TANGENCY                  20243
#define CM_MATE_CONCENTRIC                20244
#define CM_MATE_DISTANCE                  20245
#define CM_MATE_ANGLE                     20246

//------------------------------------------------------------------------------
// Collision detection with detector
//---
#define CM_COLLISION_CONSTRUCT_BVT_NODES  20260
#define CM_COLLISION_DISTANCE_QUERY       20261
#define CM_COLLISION_INTERFERENCE_SOLIDS  20262
#define CM_COLLISION_INTERFERENCE_MESHES  20263

//------------------------------------------------------------------------------
// Collision detection without detector
//---
#define CM_CROSS_BOOLEAN                  20280
#define CM_FIND_TOUCHED_FACES             20281
#define CM_SPLIT_TOUCHED_FACES            20282
#define CM_MERGE_SOLIDS                   20283

//------------------------------------------------------------------------------
// Создание объектов пользователя. Creation of new commands.       [20300,20399]
//---
#define CM_MAKEUSERCOMAND0                20300
#define CM_MAKEUSERCOMAND1                20301
#define CM_MAKEUSERCOMAND2                20302
#define CM_MAKEUSERCOMAND3                20303
#define CM_MAKEUSERCOMAND4                20304
#define CM_MAKEUSERCOMAND5                20305
#define CM_MAKEUSERCOMAND6                20306
#define CM_MAKEUSERCOMAND7                20307
#define CM_MAKEUSERCOMAND8                20308
#define CM_MAKEUSERCOMAND9                20309

//------------------------------------------------------------------------------
// Тесты многопоточности                                           [20400,20499]
//---
#define CM_BETAMULTITHREADING0            20400
#define CM_BETAMULTITHREADING1            20401
#define CM_SHELLMULTITHREADING            20410
#define CM_PROJECTIONMULTITHREADING       20411
#define CM_SHELLMULTITHREADING2           20412
#define CM_THREADSAFETY                   20413
#define CM_PARALLELPROJECTION             20414
#define CM_PARALLELPROJECTION2            20415
#define CM_PARALLELINERTIA                20416

//------------------------------------------------------------------------------
// Демонстрационные примеры. Demonstrating samples.                [20500,20699]
//---
#define CM_SAMPLE_PARAMETRIC_SKETCH       20500
#define CM_SAMPLE_WRITE_SKETCH            20501
#define CM_SAMPLE_READ_SKETCH             20502
#define CM_SAMPLE_SPINNING_BLOCK          20503
#define CM_SAMPLE_DODECAHEDRON            20504
#define CM_SAMPLE_ATTRIBUTES              20505
#define CM_SAMPLE_SOLID_TORUS_SPLINE      20506
#define CM_SAMPLE_SOLID_SPLIT_BALL        20507
#define CM_SAMPLE_CYLINDER_MANUAL_SPLIT   20508
#define CM_SAMPLE_COLLISION_DETECTION     20509
#define CM_SAMPLE_CHECK_COLLISIONS        20510
#define CM_SAMPLE_ANIMATION_OF_BENDING    20511
#define CM_SAMPLE_LOFTED_BEND             20512
#define CM_SAMPLE_SOLIDS_CLASSIFICATION   20513
#define CM_SAMPLE_CLOSE_CORNER            20514
#define CM_SAMPLE_SEVERAL_BENDS           20515
#define CM_SAMPLE_CHECK_COLLISIONS_GROUPS 20516


#define CM_APPLY_CONFIG_FILE              20517 // Применить настройки из файла конфигураций

//------------------------------------------------------------------------------
// Полигональный объект с топологией. Polymesh.                [20700,20999]
//---
#define CM_CREATE_POLYMESH                20700
#define CM_CUT_PLANE_POLYMESH             20701
#define CM_SAMPLE_POLYMESH_BUILD          20702
#define CM_CREATE_POLYMESH_BY_SHELL       20703
#define CM_REMESH_POLYMESH                20704
#define CM_SMOOTH_POLYMESH                20705
#define CM_CALC_NORMALS_POLYMESH          20706
#define CM_CUT_POLYMESH_BY_CURVES         20707
#define CM_SAMPLE_POLYMESH_CUT            20708
#define CM_SAMPLE_POLYMESH_OFFSET_PATH    20709
#define CM_POLYMESH_UNION                 20710
#define CM_POLYMESH_SUBTRACT              20711
#define CM_POLYMESH_INTERSECT             20712

//------------------------------------------------------------------------------
// Количество уникальных объектов
//---
#define CM_NUMBER_UNIQUE_SURFACE          20800

//------------------------------------------------------------------------------
// Ввод лицензионного ключа
//---
#define CM_ENTERLICENSEKEY                21000
#define CM_DISABLELICENSES                21001
#define CM_LICENSE_STATUS                 21002


#endif // __TEST_H
