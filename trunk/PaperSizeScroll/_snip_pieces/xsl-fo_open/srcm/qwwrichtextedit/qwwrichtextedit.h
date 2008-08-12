#include <QTextEdit>
#include <QTextDocument>
#include <QTextFormat>
#include <wwglobal.h>
#include <QFileIconProvider>
#include "Fo_Format.h"
#include <QSettings>

class QFontComboBox;
class QToolBar;
class QComboBox;
class QwwColorButton;

/* dorobic flagi */
class  QwwRichTextEdit : public QTextEdit {
  Q_OBJECT
public:
  QwwRichTextEdit(QWidget *parent=0);
public slots:
  void changeAlignment(QAction *a);
  void setBold(bool v);
  void setItalic(bool v);
  void setUnderline(bool v);
  void setFont_h();
  void setFont(const QFont &f);
  void setList(bool v);
  void setColor(const QColor &);
  void openfopdocument();
  void OpenFop(  const QString file  );  /* apache fop format */
  void OpenOOO(  const QString file  );  /* OpenOffice file */
  void OpenHtml(  const QString file  );
  void SaveFopFormat();
  void updateCurrentCharFormat(const QTextCharFormat &fmt);
  void updateCurrentBlockFormat();
  void fop2pdf();
  void textcolor();
  void bgcolor();


  void ShowText( QString txt );
protected:
  void AppendLayerDoc( RichDoc ldoc );
  QFileInfo CurrentFopFile;
  M_PageSize DocumentSizeFop;  /* size / margin / name */
  QSettings setter;
  QFileIconProvider  sysicon;
  bool event(QEvent *e);
  QToolBar *tb;
  QAction *ba, *ia, *ua, *opfo , *savf , *color1 , *color2;
  QAction *ac, *ar, *al, *aj , *acfoppdf;
  QAction *li;
  QFontComboBox *fcb;
  QComboBox *fsp;
  QTextList *currentList;
  void DocRootFrameDefault();
  //////QwwColorButton *colorButton;
  void resizeEvent(QResizeEvent *re);
};
