#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QGridLayout>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QSqlTableModel>
#include <QDateTime>
#include <QTranslator>

#include <QAbstractButton>
#include <QPushButton>
#include <QLabel>
#include <QMenuBar>
#include <QSettings>
#include <QStatusBar>

#include <memory>
#include <utility>

#include "gameprocess.h"
#include "ataxxplayerthread.h"
#include "iataxxplayer.h"
#include "fixedratiowidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    using Location = std::pair<int, int>;

    // declare objects
    QWidget *center;
    QGridLayout *gridLayout;

    QWidget *sidebar;
    QHBoxLayout *boxLayout;

    GameProcess game;
    IAtaxxPlayer *black;

    QPixmap *pixmapBlack;
    QPixmap *pixmapWhite;
    QPixmap *pixmapEmpty;
    QPixmap *pixmapWhiteLight;
    QPixmap *pixmapBlackLight;
    QPixmap *pixmapCloneHint;
    QPixmap *pixmapJumpHint;
    QPixmap *pixmapOldJumpPieceWhite;
    QPixmap *pixmapOldClonePieceWhite;
    QPixmap *pixmapNewPieceWhite;

    QMenu *fileMenu;
    QMenu *settingsMenu;
    QMenu *whitePlayerMenu;
    QMenu *langMenu;
    QMenu *highScoreMenu;
    QMenu *aboutMenu;

    QPushButton *restartButton;
    QPushButton *exitButton;
    FixedRatioWidget *board;

    QAction *doNewGame;
    QAction *doLoad;
    QAction *doSave;
    QAction *doClear;
    QAction *doExit;
    QAction *doAbout;

    QActionGroup *whitePlayerGroup;
    QAction *whiteHuman;
    QAction *whiteAI;

    QAction *startingPlayer;

    QAction *showScoreDialog;
    QAction *doScoreClear;

    QMessageBox *resetMsgBox;
    QSqlTableModel *model;

    QDateTime gameStart;
    QDateTime gameEnd;

    AtaxxPlayerThread *thread = 0;

    Location selected;  // if a piece had been chosen

public:
    explicit MainWindow(QWidget *parent = 0, Qt::WindowFlags flags = 0);

    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;
    // this method is called when a new translator is loaded or the system language is changed
    void changeEvent(QEvent *) override;

protected slots:
    // this slot is called by the language menu actions
    void slotLanguageChanged(QAction *action);

private:
    // loads a language by the given language shorthand (e.g. zh, ja, en)
    void loadLanguage(const QString &language);

    // creates the language menu dynamically from the contents of langPath
    void createLanguageMenu();
    void switchTranslator(QTranslator &translator, const QString &filename);

    QActionGroup *langGroup;
    QTranslator translator;     // contains the translations for this application
    QTranslator translatorQt;   // contains the translations for Qt
    QString currLang;           // contains the currently loaded language
    QString langPath;           // path of language files, always fixed to /languages


    // UI helpers
    void createGameGrid();
    void updateGameGrid();
    void clearGameGrid();
    void clickedGamePiece(unsigned int x, unsigned int y);
    bool gameIsRunning();
    void createActions();
    void createMenus();
    void createMsgBox();
    void createSideBar();
    void loadImages();

    static const QString languages[];

    // gameplay helpers
    //void clickedGamePiece(GameProcess a);
    bool isGameRunning();
    void resetGame();
    void judgeWinner();
    void setCurrentPlayerHint();

    void about();

    void readSettings();
    void writeSettings();

    void clearSavedGame();
    void loadGame();
    void saveGame();

    //void undo();

    const QString DATE_FORMAT = "yyyy-MM-dd HH:mm:ss";
    void createHighScoreModel();
    void addHighScore();

    void retranslateUi();

    void startAI();
    void setAIMove(GameProcess a);

};

#endif // MAINWINDOW_H
