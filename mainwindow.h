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

    QActionGroup *blackPlayerGroup;
    QAction *whiteHuman;
    QAction *whiteAI;

    QAction *startingPlayer;

    QActionGroup *langGroup;
    QAction *langEn;
    QAction *langJa;
    QAction *langZh;

    QTranslator *translator;

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
    void changeEvent(QEvent *) override;

private:
    void switchLanguage();

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
    void loadLanguages();

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
