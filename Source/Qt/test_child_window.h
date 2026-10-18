#ifndef __TEST_CHILD_WINDOW_H
#define __TEST_CHILD_WINDOW_H


#include <test_style.h>
#include <QWidget>
#include <QGLWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>


QT_BEGIN_NAMESPACE
class QCloseEvent;
class QPaintEvent;
class QKeyEvent;
class QMouseEvent;
class QWheelEvent;
class QResizeEvent;
QT_END_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
//
//
//
//
////////////////////////////////////////////////////////////////////////////////
class SelectRectangleItem : public QGraphicsRectItem
{
  enum Direction
  {
    dir_NormalFrame,  // нормальное направление рамки
    dir_ReverseFrame, // обратное направление рамки
  };
public:
  SelectRectangleItem();
  virtual ~SelectRectangleItem();

public:
  void setCoordinates( const QPoint& begin, const QPoint& end );

public:
  virtual void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr );

protected:
  QColor m_colorNormalSelect;
  QColor m_colorNormalFillSelect;

  QColor m_colorReverselSelect;
  QColor m_colorReverseFillSelect;

  QPoint m_begin, m_end;

private:
  Q_DISABLE_COPY(SelectRectangleItem)
};

////////////////////////////////////////////////////////////////////////////////
//
//
//
//
////////////////////////////////////////////////////////////////////////////////
class GraphicsScene : public QGraphicsScene
{
  Q_OBJECT
public:
  explicit GraphicsScene( QObject* parent = nullptr );
  virtual ~GraphicsScene();

protected:
  static bool shiftPress;
  static bool ctrlPress;
  static bool capslockPress;
public:
  static bool  IsShiftPress()     { return shiftPress; }
  static bool  IsCtrlPress()      { return ctrlPress; }
  static bool  IsCapslockPress()  { return capslockPress; }

protected:
  void renderWorld( QPainter* painter );
  void mouseMoveCommand( POINT& p, bool lBtn, bool mBtn );
  void mouseCommand( uint message, POINT& p, int paramCount = 0 );
  void keyboardCommand( int key );

protected:
  virtual void drawBackground(QPainter *painter, const QRectF &rect);
  virtual bool event( QEvent *event );
  virtual void keyPressEvent( QKeyEvent* event );
  virtual void mouseDoubleClickEvent( QGraphicsSceneMouseEvent* event );
  virtual void mouseMoveEvent( QGraphicsSceneMouseEvent* event );
  virtual void mousePressEvent( QGraphicsSceneMouseEvent* event );
  virtual void mouseReleaseEvent( QGraphicsSceneMouseEvent* event );
  virtual void wheelEvent( QGraphicsSceneWheelEvent* event );
  virtual void focusInEvent(QFocusEvent* event);

protected:
  SelectRectangleItem* rectangleItem;
  QPoint pntBegin;

private:
  Q_DISABLE_COPY(GraphicsScene)
};

////////////////////////////////////////////////////////////////////////////////
//
//
//
//
////////////////////////////////////////////////////////////////////////////////
class GraphicsView : public QGraphicsView
{
  Q_OBJECT
public:
  GraphicsView();
  virtual ~GraphicsView();

protected:
  virtual bool event( QEvent *event );
  virtual void closeEvent( QCloseEvent* event );
  virtual void resizeEvent(QResizeEvent* event);

private:
  Q_DISABLE_COPY(GraphicsView)
};



#endif // __TEST_CHILD_WINDOW_H
