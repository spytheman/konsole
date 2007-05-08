/*
    This file is part of Konsole, an X terminal.
    Copyright (C) 1996 by Matthias Ettrich <ettrich@kde.org>
    Copyright (C) 1997,1998 by Lars Doelle <lars.doelle@on-line.de>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
    02110-1301  USA.
*/

#ifndef KONSOLE_H
#define KONSOLE_H

// Qt 
#include <QStringList>
#include <Q3PtrDict>
#include <Q3PtrList>
#include <QList>

// KDE
#include <kmainwindow.h>
#include <kdialog.h>

// Konsole
#include "Pty.h"
#include "Vt102Emulation.h"
#include "Session.h"
#include "schema.h"
#include "konsolebookmarkhandler.h"

#undef PACKAGE
#undef VERSION
#define PACKAGE "konsole"
#define VERSION "1.9"


class QCheckBox;
class QEvent;
class QLabel;
class QToolButton;
class QWidgetAction;
class QSignalMapper;

class KAction;
class KColorCells;
class KFileDialog;
class KFindDialog;
class KMenu;
class KSelectAction;
class KConfig;
class KTabWidget;
class KTemporaryFile;
class KToggleAction;
class KWinModule;

class TerminalDisplay;
class Profile;
class SessionManager;
class ViewSplitter;

class KonsoleAdaptor;
class Konsole : public KMainWindow
{
    Q_OBJECT

    friend class KonsoleSessionManaged;
public:

  Konsole(const char * name, int histon, bool menubaron, bool tabbaron,
    bool frameon, bool scrollbaron,
    const QString &type = "", bool b_inRestore = false, const int wanted_tabbar = 0,
    const QString &workdir=QString());

  ~Konsole();
  void setColLin(int columns, int lines);
  void setAutoClose(bool on);
  void initFullScreen();
  void initSessionFont(int fontNo);
  void initSessionFont(QFont f);
  void initSessionKeyTab(const QString &keyTab);
  void initMonitorActivity(bool on);
  void initMonitorSilence(bool on);
  void initMasterMode(bool on);
  void initTabColor(QColor color);
  void initHistory(int lines, bool enable);
//  void newSession(const QString &program, const QStringList &args, const QString &term, const QString &icon, const QString &title, const QString &cwd);
  void setSchema(const QString & path);
  void setEncoding(int);
  void setSessionTitle(QString&, Session* = 0);
  void setSessionEncoding(const QString&, Session* = 0);

  void enableFullScripting(bool b);
  void enableFixedSize(bool b);

  void setDefaultSession(const QString &filename);
  void showTipOnStart();

  // Additional functions for DCOP
  int sessionCount() { return sessions.count(); }

  QString currentSession();
  QString newSession(const QString &type);
  Session* newSession(Profile* type);
  QString sessionId(const int position);

  void activateSession(const QString& sessionId);
  void feedAllSessions(const QString &text);
  void sendAllSessions(const QString &text);

  KUrl baseURL() const;
  void callReadPropertiesInternal(KConfig *config, int number) { readPropertiesInternal(config,number); }

  enum TabPosition { TabNone, TabTop, TabBottom };
  enum TabViewModes { ShowIconAndText = 0, ShowTextOnly = 1, ShowIconOnly = 2 };

  /** Sets a session manager used to create and manage running sessions in this widget */
  void setSessionManager(SessionManager* manager);
  /** Returns the active session manager */
  SessionManager* sessionManager();

public Q_SLOTS:
  void activateSession(int position);
  void activateSession(QWidget*);
  void slotUpdateSessionConfig(Session *session);
  void slotResizeSession(Session*, QSize);
  void slotSetSessionEncoding(Session *session, const QString &encoding);
  void slotGetSessionSchema(Session *session, QString &schema);
  void slotSetSessionSchema(Session *session, const QString &schema);

  void makeGUI();
  QString newSession();
protected:

 bool queryClose();
 // \reimp
 void saveProperties(KConfigGroup& config);
 // \reimp
 void readProperties(const KConfigGroup& config);




private Q_SLOTS:
  void configureRequest(TerminalDisplay*,int,int,int);
  void activateSession();
  void activateSession(Session*);
  void closeCurrentSession();
  void confirmCloseCurrentSession(Session* _se=0);
  void doneSession(Session*);
  void toggleFullScreen();
  bool fullScreen();
  void setFullScreen(bool on);
  void schema_menu_activated(int item);
  void pixmap_menu_activated(int item, TerminalDisplay* tewidget=0);
  void keytab_menu_activated(int item);
  void schema_menu_check();
  void attachSession(Session*);
  void slotDetachSession();
  void bookmarks_menu_check();
  
  void slotNewSessionAction(QAction* action);
  
  void updateSchemaMenu();
  void updateKeytabMenu();

  void changeTabTextColor(Session*, int);
  void changeColumns(int);
  void changeColLin(int columns, int lines);
  void notifySessionState(Session* session,int state);
  void notifySize(int columns, int lines);
  void updateTitle();
  void prevSession();
  void nextSession();
  void activateMenu();
  void slotMovedTab(int,int);
  void moveSessionLeft();
  void moveSessionRight();
  void allowPrevNext();
  void setSchema(int n, TerminalDisplay* tewidget);   // no slot necessary?
  void sendSignal(QAction*);
  void slotClearTerminal();
  void slotResetClearTerminal();
  void slotSelectTabbar();
  void slotToggleMenubar();
  void slotRenameSession();
  void slotRenameSession(Session* ses, const QString &name);
  void slotToggleMonitor();
  void slotToggleMasterMode();
  void slotClearAllSessionHistories();
  void slotHistoryType();
  void slotClearHistory();
  void slotFindHistory();
  void slotSaveHistory();
  void slotShowSaveHistoryDialog();
  void slotSelectBell();
  void slotSelectSize();
  void slotSelectFont();
  void slotSelectScrollbar();
  void loadScreenSessions();
  void updateFullScreen(bool on);

  void slotSaveSettings();
  void slotSaveSessionsProfile();
  void slotConfigureNotifications();
  void slotConfigureKeys();
  void slotConfigure();
  void reparseConfiguration();

  void disableMasterModeConnections();
  void enableMasterModeConnections();
  void enterURL( const QString&, const QString& );
  void newSession( const QString&, const QString& );

  void slotFind();
  void slotFindDone();
  void slotFindNext();
  void slotFindPrevious();

  void showTip();

  void slotSetSelectionEnd() { te->setSelectionEnd(); }
  void slotCopyClipboard() { te->copyClipboard(); }
  void slotPasteClipboard() { te->pasteClipboard(); }
  void slotPasteSelection() { te->pasteSelection(); }

  void listSessions();
  void switchToSession();

  void biggerFont();
  void smallerFont();

  void slotZModemDetected(Session *session);
  void slotZModemUpload();

  void slotPrint();

  void toggleBidi();

  void slotTabContextMenu(QWidget*, const QPoint &);
  void slotTabDetachSession();
  void slotTabRenameSession();
  /** Close the tab whoose popup menu the user has selected */
  void slotTabCloseSession();
  /** Close the tab containing the specified widget. */
  void slotTabCloseSession(QWidget* sessionWidget);
  void slotTabToggleMonitor();
  void slotTabToggleMasterMode();
  void slotTabbarContextMenu(const QPoint &);
  void slotTabSetViewOptions(int);
  void slotTabbarToggleDynamicHide();
  void slotToggleAutoResizeTabs();
  void slotFontChanged();
  void slotToggleSplitView(bool split);

  void slotSetEncoding();
private:
  KConfig *defaultSession();
/*  QString newSession(KConfig *co, QString pgm = QString(), const QStringList &args = QStringList(),
                     const QString &_term = QString(), const QString &_icon = QString(),
                     const QString &_title = QString(), const QString &_cwd = QString()); */
  void readProperties(const KConfigGroup &config, const QString &schema, bool globalConfigOnly, bool readGlobalConfig);
  void applySettingsToGUI();
  void makeTabWidget();
  void makeBasicGUI();
  void runSession(Session* s);
  void addSession(Session* s);
  void detachSession(Session* _se=0);
  void setColorPixmaps();
  void renameSession(Session* ses);
  void savePropertiesHelper(KConfigGroup& config);

  void setSchema(ColorSchema* s, TerminalDisplay* tewidget=0);
  void setMasterMode(bool _state, Session* _se=0);

  void buildSessionMenus();
  void addSessionCommand( Profile* info );
  void loadSessionCommands();
  void createSessionMenus();
  void addScreenSession(const QString & path, const QString & socket);
  void resetScreenSessions();

  TerminalDisplay* createSessionView();
  void createViews(Session* session);

//  void initTerminalDisplay(TerminalDisplay* new_te, TerminalDisplay* default_te);

  void createSessionTab(TerminalDisplay *widget, const QIcon& iconSet,
                        const QString &text, int index = -1);
  QIcon iconSetForSession(Session *session) const;

  bool eventFilter( QObject *o, QEvent *e );

  Q3PtrList<TerminalDisplay> activeTEs();

  Q3PtrDict<Session> action2session;
  Q3PtrDict<KToggleAction> session2action;

  Q3PtrList<Session> sessions;

  QList<KTemporaryFile*> tempfiles;
  KConfig* m_defaultSession;
  QString m_defaultSessionFilename;

  //SessionTabWidget* tabwidget;
  // KTabWidget*    tabwidget;
  TerminalDisplay*      te;     // the visible TerminalDisplay, either sole one or one of many
  Session*     se;
  Session*     se_previous;
  Session*     m_initialSession;
  ColorSchemaList* colors;
  QString        s_encodingName;

  KWinModule*    kWinModule;

  KMenuBar*   menubar;
  KStatusBar* statusbar;

  KMenu* m_session;
  KMenu* m_edit;
  KMenu* m_view;
  KMenu* m_bookmarks;
  KMenu* m_bookmarksSession;
  KMenu* m_options;
  KMenu* m_schema;
  KMenu* m_keytab;
  KMenu* m_tabbarSessionsCommands;
  KMenu* m_signals;
  KMenu* m_help;
  KMenu* m_rightButton;
  KMenu* m_sessionList;

  //-- Session Tabs Context Menu -------
  void setupTabContextMenu();

  KMenu* m_tabPopupMenu;
  KMenu* m_tabPopupTabsMenu;
  KMenu* m_tabbarPopupMenu;
  KToggleAction* m_tabMonitorActivity;
  KToggleAction* m_tabMonitorSilence;
  KToggleAction* m_tabMasterMode;
  //------------------------------------


  KAction *m_zmodemUpload;
  KToggleAction *monitorActivity;
  KToggleAction *monitorSilence;
  KToggleAction *masterMode;

  KAction* moveSessionLeftAction;
  KAction* moveSessionRightAction;

  KToggleAction *showMenubar;
  KToggleAction *m_fullscreen;

  KSelectAction *selectSize;
  KSelectAction *selectFont;
  KSelectAction *selectScrollbar;
  KSelectAction *selectTabbar;
  KSelectAction *selectBell;
  KSelectAction *selectSetEncoding;
  

  KAction       *m_clearHistory;
  KAction       *m_findHistory;
  KAction       *m_findNext;
  KAction       *m_findPrevious;
  KAction       *m_saveHistory;
  KAction       *m_detachSession;

  KAction       *m_copyClipboard;
  KAction       *m_pasteClipboard;
  KAction       *m_pasteSelection;
  KAction       *m_clearTerminal;
  KAction       *m_resetClearTerminal;
  KAction       *m_clearAllSessionHistories;
  KAction       *m_renameSession;
  KAction       *m_saveProfile;
  KAction       *m_closeSession;
  KAction       *m_print;
  KAction       *m_quit;
  KAction       *m_tabDetachSession;

  KActionCollection *m_shortcuts;

  QActionGroup  *m_sessionGroup;

  BookmarkHandler *bookmarkHandler;
  BookmarkHandler *bookmarkHandlerSession;

  KFindDialog* m_finddialog;
  KFileDialog* saveHistoryDialog;
  bool         m_find_first;
  bool         m_find_found;
  QString      m_find_pattern;

  int cmd_serial;
  int cmd_first_screen;
  int         n_keytab;
  int         n_defaultKeytab;
  int         n_scroll;
  int         n_tabbar;
  int         n_bell;
  int         n_render;
  int         curr_schema; // current schema no
  int         wallpaperSource;
  int         sessionIdCounter;
  int         monitorSilenceSeconds;

  QString     s_schema;
  QString     s_kconfigSchema;
  QString     s_word_seps;			// characters that are considered part of a word
  QString     pmPath; // pixmap path
  QString     dropText;
  QFont       defaultFont;
  QSize       defaultSize;

  QRect       _saveGeometry;

  QTimer      m_closeTimeout;
  bool        _closing; //Set to true when the applications begins to close

  TabViewModes m_tabViewMode;
  bool        b_dynamicTabHide;
  bool        b_autoResizeTabs;

  bool        b_framevis:1;
  bool        b_fullscreen:1;
  bool        m_menuCreated:1;
  bool        b_warnQuit:1;
  bool        isRestored:1;
  bool        b_allowResize:1; // Whether application may resize
  bool        b_fixedSize:1; // Whether user may resize
  bool        b_addToUtmp:1;
  bool        b_bidiEnabled:1;

  bool        b_histEnabled:1;
  bool        b_fullScripting:1;
  bool        b_showstartuptip:1;
  bool        b_sessionShortcutsEnabled:1;
  bool        b_sessionShortcutsMapped:1;
  bool        b_matchTabWinTitle:1;

  unsigned int m_histSize;
  int m_separator_id;

  Session*  m_contextMenuSession;

  QToolButton* m_newSessionButton;
  QToolButton* m_removeSessionButton;
  QPoint      m_newSessionButtonMousePressPos;

  QSignalMapper* sessionNumberMapper;
  QStringList    sl_sessionShortCuts;
  QString  s_workDir;

  QColor    m_tabColor;

  SessionManager* _sessionManager;

  ViewSplitter* _view;
  friend class KonsoleAdaptor;
};

class QSpinBox;

class HistoryTypeDialog : public KDialog
{
    Q_OBJECT
public:
  HistoryTypeDialog(const HistoryType& histType,
                    unsigned int histSize,
                    QWidget *parent);

public Q_SLOTS:

  void slotHistEnable(bool);
  void slotDefault();
  void slotSetUnlimited();

  unsigned int nbLines() const;
  bool isOn() const;

protected:
  QLabel*        m_label;
  QSpinBox*      m_size;
  QCheckBox*     m_btnEnable;
  QPushButton*   m_setUnlimited;
};

class SizeDialog : public KDialog
{
    Q_OBJECT
public:
  SizeDialog(unsigned int const columns,
             unsigned int const lines,
             QWidget *parent);

public Q_SLOTS:
  void slotDefault();

  unsigned int columns() const;
  unsigned int lines() const;

protected:
  QSpinBox*  m_columns;
  QSpinBox*  m_lines;
};

#endif