#include "mainwindow.h"

#include <QEvent>
#include <QCloseEvent>
#include <QApplication>
#include <QSqlQuery>
#include <QSqlRecord>

#include <memory>
#include <sstream>

#include "clickablelabel.h"
#include "gameprocess.h"
#include "gameai.h"
#include "highscore_columns.h"
#include "highscoredialog.h"
#include "utils.h"
#include "aiataxxplayer.h"
#include "botzonerunner.h"

const QString MainWindow::languages[] = {
    "en_US", "ja_JP", "zh_TW"
};

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags),
      game(GameProcess())
{
    setObjectName("MainWindow");

    black = nullptr;
    selected = Location(-1, -1);

    createActions();
    createMenus();
    createMsgBox();

    loadImages();

    createSideBar();
    createGameGrid();

    readSettings();

    createHighScoreModel();

    loadLanguages();
    retranslateUi();

    statusBar()->showMessage(tr("Ready"));

    resetGame();
}

void MainWindow::switchLanguage()
{
    // TODO implement translation
    if (langEn->isChecked())
        qApp->removeTranslator(translator);
    else
        qApp->installTranslator(translator);
}

void MainWindow::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::LanguageChange)
        retranslateUi();
    else
        QMainWindow::changeEvent(e);
}

void MainWindow::retranslateUi()
{
    setWindowTitle(tr("Ataxx Main Window"));
    fileMenu->setTitle(tr("&File"));
    settingsMenu->setTitle(tr("&Settings"));
    whitePlayerMenu->setTitle(tr("&White Player"));
    langMenu->setTitle(tr("&Languages"));
    highScoreMenu->setTitle(tr("&High Score"));
    aboutMenu->setTitle(tr("&About"));

    doNewGame->setText(tr("&New Game"));
    doExit->setText(tr("E&xit"));
    doLoad->setText(tr("&Load Game"));
    doSave->setText(tr("&Save Game"));

    restartButton->setText(tr("Restart"));
    exitButton->setText(tr("Exit"));
}

void MainWindow::loadImages()
{
    pixmapBlack = new QPixmap;
    if (!pixmapBlack->load(":/resources/black.png"))
        qWarning("Failed to load black.png");

    pixmapWhite = new QPixmap;
    if (!pixmapWhite->load(":/resources/white.png"))
        qWarning("Failed to load white.png");

    pixmapEmpty = new QPixmap;
    if (!pixmapEmpty->load(":/resources/empty.png"))
        qWarning("Failed to load empty.png");

    pixmapBlackLight = new QPixmap;
    if (!pixmapBlackLight->load(":/resources/black_light.png"))
        qWarning("Failed to load black_light.png");

    pixmapWhiteLight = new QPixmap;
    if (!pixmapWhiteLight->load(":/resources/white_light.png"))
        qWarning("Failed to load white_light.png");
}

void MainWindow::loadLanguages()
{
    translator = new QTranslator;
    for (auto &i : languages)
    {
        if (!translator->load(i, ":/resources"))
        {
            QByteArray arr = ("Failed to load " + i + " language").toUtf8();
            qWarning(arr.data());
        }
    }
}

MainWindow::~MainWindow()
{
    if (black)
        delete black;
}

void MainWindow::startAI()
{
    if (thread && thread->isRunning())
        qDebug("AI still calculating");
    else
    {
        black = new AIAtaxxPlayer;
        thread = new AtaxxPlayerThread(game, black);
        connect(thread, &AtaxxPlayerThread::finished, this, &MainWindow::setAIMove);
        qDebug("AI start to calculate");
        //BotzoneRunner::debug(game, "in startAI, before thread start");
        thread->start();
    }
}

void MainWindow::setAIMove(GameProcess a)
{
    thread->wait();
    //BotzoneRunner::debug(a, "in setAIMove");
    game = a;

    // TODO implement undo

    updateGameGrid();
    judgeWinner();
    setCurrentPlayerHint();
}

void MainWindow::createSideBar()
{
    sidebar = new QWidget(this);
    sidebar->setStyleSheet("background-color: gray;");
    sidebar->setMinimumWidth(200);

    restartButton = new QPushButton("Restart", sidebar);
    restartButton->setGeometry(QRect(QPoint(50, 50), QSize(100, 50)));
    restartButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    connect(restartButton, &QPushButton::clicked, [this](bool clicked)
    {
        if (!isGameRunning() || (resetMsgBox->exec() == QMessageBox::Ok))
        {
            game = GameProcess(startingPlayer->isChecked());
            resetGame();
            BotzoneRunner::debug(game, "GAME HAS BEEN RESET");
        }
    });

    exitButton = new QPushButton("Exit", sidebar);
    exitButton->setGeometry(QRect(QPoint(50, 100), QSize(100, 50)));
    exitButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(exitButton, &QPushButton::clicked, this, &MainWindow::close);
}

void MainWindow::createGameGrid()
{
    gridLayout = new QGridLayout;
    gridLayout->setSpacing(0);
    gridLayout->setMargin(0);

    for (unsigned int x = 0; x < GameProcess::BOARD_SIZE; x ++)
        for (unsigned int y = 0; y < GameProcess::BOARD_SIZE; y ++)
        {
            ClickableLabel *label = new ClickableLabel(this);
            label->setMinimumSize(50, 50);
            label->setFrameStyle(QFrame::Panel | QFrame::Sunken);
            label->setAlignment(Qt::AlignCenter);
            label->setPixmap(*pixmapEmpty);

            connect(label, &ClickableLabel::clicked,
                    [this, x, y, label]
            {
                clickedGamePiece(x, y);
            });

            gridLayout->addWidget(label, y, x);
        }

    boxLayout = new QHBoxLayout;
    board = new FixedRatioWidget;     // the board itself
    board->setLayout(gridLayout);
    boxLayout->addWidget(board);
    boxLayout->addWidget(sidebar);

    center = new QWidget(this);       // wrapper to put everything at the center
    center->setLayout(boxLayout);

    setCentralWidget(center);
}

void MainWindow::updateGameGrid()
{
    for (unsigned int x = 0; x < GameProcess::BOARD_SIZE; x ++)
        for (unsigned int y = 0; y < GameProcess::BOARD_SIZE; y ++)
        {
            QLayoutItem *item = gridLayout->itemAtPosition(y, x);
            auto label = dynamic_cast<ClickableLabel *>(item->widget());

            switch (game.getPiece(x, y))
            {
            case GameProcess::X:
                if (selected == Location(x, y))
                    label->setPixmap(*pixmapBlackLight);
                else
                    label->setPixmap(*pixmapBlack);
                break;
            case GameProcess::O:
                if (selected == Location(x, y))
                    label->setPixmap(*pixmapWhiteLight);
                else
                    label->setPixmap(*pixmapWhite);
                break;
            case GameProcess::Absent:
                label->setPixmap(*pixmapEmpty);
                break;
            }

            // TODO process displaying available move locations

            label->setScaledContents(true);
        }
}

void MainWindow::clearGameGrid()
{
    for (unsigned int x = 0; x < GameProcess::BOARD_SIZE; x ++)
        for (unsigned int y = 0; y < GameProcess::BOARD_SIZE; y ++)
        {
            QLayoutItem *item = gridLayout->itemAtPosition(y, x);
            auto label = dynamic_cast<ClickableLabel *>(item->widget());
            label->setPixmap(*pixmapEmpty);
        }
}

void MainWindow::clickedGamePiece(unsigned int x, unsigned int y)
{
    if (thread && thread->isRunning())          // AI is still calculating
        return;

    if (game.getPiece(x, y)
            == (game.player ? GameProcess::X : GameProcess::O))   // a valid location was clicked on
    {
        if (selected == Location(x, y)) // the user clicked on the same piece twice
            selected = Location(-1, -1);
        else
            selected = Location(x, y);
    }
    else
    {
        if (game.isValidMove(selected.first, selected.second, x, y))
        {
            if (game.isEmpty())
                gameStart = QDateTime::currentDateTime();
            qDebug(QString("Current player is %1").arg(game.player).toStdString().c_str());
            game.doMove(selected.first, selected.second, x, y);
            selected = Location(-1, -1);
            setCurrentPlayerHint();
            updateGameGrid();
            judgeWinner();

            qDebug(QString("Current player is %1").arg(game.player).toStdString().c_str());
            if (black != nullptr && !game.player && game.isMoveAvailable())
            {
                startAI();
            }
            qDebug(QString("AI finished. Current player is %1").arg(game.player).toStdString().c_str());
        }
        else
            selected = Location(-1, -1);                         // clear the selection
    }

    updateGameGrid();
    judgeWinner();

}

void MainWindow::judgeWinner()
{
    if (!game.isMoveAvailable())
    {
        gameEnd = QDateTime::currentDateTime();
        int score = -game.getScore();              // when the game is over the current player lost
        bool winner = !game.player;

        statusBar()->showMessage(tr("The game is over!"));

        addHighScore();

        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setWindowTitle(tr("The game is over!"));

        if (winner)
            msgBox.setText("Black won this round!");
        else
            msgBox.setText("White won this round!");

        msgBox.setInformativeText(tr("Score is %1. Restart or close?").arg(score));

        QPushButton *startNewButton = msgBox.addButton(tr("Restart"), QMessageBox::ActionRole);
        QPushButton *closeButton = msgBox.addButton(QMessageBox::Close);

        msgBox.exec();

        if (qobject_cast<QPushButton *>(msgBox.clickedButton()) == startNewButton)
        {
            game = GameProcess(startingPlayer->isChecked());
            resetGame();
        }
        else if (qobject_cast<QPushButton *>(msgBox.clickedButton()) == closeButton)
        {
            writeSettings();
            this->close();
        }
    }
}

void MainWindow::setCurrentPlayerHint()
{
    statusBar()->showMessage(QString(game.player ? "Black" : "White") + "'s turn.");
}

void MainWindow::createActions()
{
    doNewGame = new QAction(tr("&New game"), this);
    doNewGame->setShortcuts(QKeySequence::New);
    connect(doNewGame, &QAction::triggered, [this]
    {
        if (!isGameRunning() || (resetMsgBox->exec() == QMessageBox::Ok))
        {
            game = GameProcess(startingPlayer->isChecked());
            resetGame();
        }
    });

    doExit = new QAction(tr("E&xit"), this);
    doExit->setShortcuts(QKeySequence::Quit);
    connect(doExit, &QAction::triggered, this, &MainWindow::close);

    doLoad = new QAction(tr("&Load game"), this);
    doLoad->setShortcuts(QKeySequence::Open);
    doLoad->setStatusTip(tr("Load the saved game"));
    connect(doLoad, &QAction::triggered, this, &MainWindow::loadGame);

    doSave = new QAction(tr("&Save game"), this);
    doSave->setShortcuts(QKeySequence::Save);
    doSave->setStatusTip(tr("Save the current game"));
    connect(doSave, &QAction::triggered, this, &MainWindow::saveGame);

    doClear = new QAction(tr("&Clear saved game"), this);
    doClear->setStatusTip(tr("Clear the saved game"));
    connect(doClear, &QAction::triggered, this, &MainWindow::clearSavedGame);

    showScoreDialog = new QAction(tr("&Show high score"), this);
    showScoreDialog->setStatusTip(tr("Show saved high scores"));
    connect(showScoreDialog, &QAction::triggered, [this]
    {
        HighScoreDialog dialog(model);          // create the dialog with the SQL model
        dialog.exec();
    });

    doScoreClear = new QAction(tr("&Clear high score"), this);
    doScoreClear->setStatusTip(tr("Clear saved high scores"));
    connect(doScoreClear, &QAction::triggered, [this]
    {
        QSqlQuery query;
        query.exec("DELETE FROM scores");
        model->select();
    });

    whiteHuman = new QAction(tr("Human"), this);
    whiteHuman->setCheckable(true);
    whiteHuman->setStatusTip(tr("Human as white player"));
    connect(whiteHuman, &QAction::triggered, [this]
    {
        black = nullptr;
    });

    whiteAI = new QAction(tr("AI"), this);
    whiteAI->setCheckable(true);
    whiteAI->setStatusTip(tr("AI as white player"));
    connect(whiteAI, &QAction::toggled, [this]
    {
        black = new AIAtaxxPlayer;
        if (!game.player)
            startAI();
    });

    whitePlayerGroup = new QActionGroup(this);
    whitePlayerGroup->addAction(whiteHuman);
    whitePlayerGroup->addAction(whiteAI);
    whiteHuman->setChecked(true);

    startingPlayer = new QAction(tr("Black starts"), this);
    startingPlayer->setCheckable(true);
    startingPlayer->setChecked(true);
    connect(startingPlayer, &QAction::toggled, [this](bool checked)
    {
        if (!isGameRunning() || resetMsgBox->exec() == QMessageBox::Ok)
        {
            game = GameProcess(checked);
            resetGame();
        }
    });

    doAbout = new QAction(tr("&About"), this);
    doAbout->setStatusTip(tr("About this application"));
    connect(doAbout, &QAction::triggered, this, &MainWindow::about);

    langEn = new QAction(tr("en_US"), this);
    langEn->setCheckable(true);
    langEn->setStatusTip(tr("Change language to en_US"));
    connect(langEn, &QAction::triggered, this, &MainWindow::switchLanguage);

    langJa = new QAction(tr("ja_JP"), this);
    langJa->setCheckable(true);
    langJa->setStatusTip(tr("Change language to ja_JP"));
    connect(langJa, &QAction::triggered, this, &MainWindow::switchLanguage);

    langZh = new QAction(tr("zh_CN"), this);
    langZh->setCheckable(true);
    langZh->setStatusTip(tr("Change language to zh_CN"));
    connect(langZh, &QAction::triggered, this, &MainWindow::switchLanguage);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(doNewGame);
    fileMenu->addAction(doLoad);
    fileMenu->addAction(doSave);
    fileMenu->addAction(doClear);
    fileMenu->addSeparator();
    fileMenu->addAction(doExit);

    settingsMenu = menuBar()->addMenu(tr("&Settings"));

    whitePlayerMenu = settingsMenu->addMenu(tr("&White player"));
    whitePlayerMenu->addAction(whiteHuman);
    whitePlayerMenu->addAction(whiteAI);

    langMenu = settingsMenu->addMenu(tr("&Language"));
    langMenu->addAction(langEn);
    langMenu->addAction(langJa);
    langMenu->addAction(langZh);

    settingsMenu->addAction(startingPlayer);

    highScoreMenu = menuBar()->addMenu(tr("&High score"));
    highScoreMenu->addAction(showScoreDialog);
    highScoreMenu->addAction(doScoreClear);

    aboutMenu = menuBar()->addMenu(tr("&About"));
    aboutMenu->addAction(doAbout);

}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Ataxx"),
                       tr("<b>Ataxx</b> with C++14 and Qt 5. Explore to find out more!"));
}

void MainWindow::resetGame()
{
    clearGameGrid();
    updateGameGrid();
    setCurrentPlayerHint();
    if (!startingPlayer->isChecked() && whiteAI->isChecked())   // if the starting player is AI
    {
        gameStart = QDateTime::currentDateTime();
        startAI();
    }
    setCurrentPlayerHint();
}

bool MainWindow::isGameRunning()
{
    return !game.isEmpty() && game.isMoveAvailable();
}

void MainWindow::createMsgBox()
{
    resetMsgBox = new QMessageBox(this);
    resetMsgBox->setIcon(QMessageBox::Warning);
    resetMsgBox->setText(tr("Reset running game"));
    resetMsgBox->setInformativeText(tr("Do you want to reset the current game to apply changes?"));
    resetMsgBox->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
}

void MainWindow::readSettings()
{
    QSettings settings;
    QPoint pos = settings.value("Position", QPoint(200, 200)).toPoint();
    QSize size = settings.value("Size", QSize(800, 650)).toSize();
    bool starting = settings.value("StartingPlayer", true).toBool();
    bool withAI = settings.value("WithAI", true).toBool();
    resize(size);
    move(pos);
    startingPlayer->setChecked(starting);
    if (withAI)
        whiteAI->setChecked(true);
    else
        whiteHuman->setChecked(true);
}


void MainWindow::writeSettings()
{
    QSettings settings;
    settings.setValue("Position", pos());
    settings.setValue("Size", size());
    settings.setValue("StartingPlayer", startingPlayer->isChecked());
    settings.setValue("WithAI", whiteAI->isChecked());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (isGameRunning())
    {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Warning);
        msg.setWindowTitle(tr("Exiting Ataxx"));
        msg.setText(tr("Save running game"));
        msg.setInformativeText(tr("Do you want to save your running game?"));
        msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        msg.setDefaultButton(QMessageBox::Cancel);
        auto response = msg.exec();
        if (response == QMessageBox::Yes)
        {
            saveGame();
            model->submitAll();
            writeSettings();
            event->accept();
        }
        if (response == QMessageBox::Cancel)
        {
            event->ignore();
        }
    }

}

void MainWindow::saveGame()
{
    QSettings settings;
    settings.beginGroup("GameState");
    GameProcess::StoreData data = game.storeProcess();
    settings.setValue("BoardData", QString::fromStdString(data.first));
    settings.setValue("BoardDataHash", QString::number(data.second));
    settings.endGroup();
}

void MainWindow::loadGame()
{
    QSettings settings;

    if (settings.contains("GameState/BoardData"))
    {
        settings.beginGroup("GameState");
        std::string str = settings.value("BoardData").toString().toStdString();
        size_t hash = settings.value("BoardDataHash").toULongLong();
        bool result = game.loadProcess(GameProcess::StoreData(str, hash));
        settings.endGroup();
        resetGame();
        if (!result)
            QMessageBox::warning(this, tr("Game loading failed"), tr("The save data seems to be corrupted. A new game will be created."));
        else
            QMessageBox::information(this, tr("Game loading finished"), tr("The saved game has been successfully loaded."));
    }
}

void MainWindow::clearSavedGame()
{
    QSettings settings;
    settings.beginGroup("GameState");
    settings.remove("");
    settings.endGroup();
}

void MainWindow::createHighScoreModel()
{
    model = new QSqlTableModel(this);
    model->setTable("scores");
    model->select();
}

void MainWindow::addHighScore()
{
    QSqlRecord record = model->record();
    record.setValue(as_integer(Columns::start), gameStart.toString(DATE_FORMAT));
    record.setValue(as_integer(Columns::end), gameEnd.toString(DATE_FORMAT));
    record.setValue(as_integer(Columns::startingPlayer), startingPlayer->isChecked() ? "Black" : "White");
    int score = -game.getScore();
    bool winner = !game.player;
    record.setValue(as_integer(Columns::playerWon), winner ? "Black" : (whiteAI->isChecked() ? "White (AI)" : "White"));
    record.setValue(as_integer(Columns::score), score);
    model->insertRecord(-1, record);
}
