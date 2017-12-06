#include "mainwindow.h"

#include <QEvent>
#include <QCloseEvent>
#include <QApplication>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDir>

#include <memory>
#include <sstream>
#include <cstdlib>

#include "clickablelabel.h"
#include "gameprocess.h"
#include "gameai.h"
#include "highscore_columns.h"
#include "highscoredialog.h"
#include "utils.h"
#include "aiataxxplayer.h"
#include "botzonerunner.h"

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags),
      game(GameProcess())
{
    (void)GameAI::timeToQuit;
    (void)GameAI::threadCount;

    setObjectName("MainWindow");
    setWindowTitle(tr("Ataxx Main Window"));

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

    statusBar()->showMessage(tr("Ready"));

    resetGame();
}

void MainWindow::changeEvent(QEvent *e)
{
    if (e)
    {
        QString locale;
        switch (e->type())
        {
        case QEvent::LanguageChange:
            // event will be sent if a translator is loaded
            retranslateUi();
            break;

        case QEvent::LocaleChange:
            // event will be sent if the system locale changes
            locale = QLocale::system().name();
            loadLanguage(locale);
            break;

        default:
            ;
        }
        QMainWindow::changeEvent(e);
    }
}

void MainWindow::retranslateUi()
{
    writeSettings();            // save the current settings so that the window size doesn't mutate
    setWindowTitle(tr("Ataxx Main Window"));
    // clear the menuBar() so that we don't have duplicate entries
    menuBar()->clear();
    createActions();
    createMenus();
    createMsgBox();
    createSideBar();
    createGameGrid();           // boxLayout needs to be recreated so that the locale in the sidebar can be reloaded
    readSettings();             // restore the settings checked
    resetGame();                // redraw the game board
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

    pixmapCloneHint = new QPixmap;
    if (!pixmapCloneHint->load(":/resources/clone_hint.png"))
        qWarning("Failed to load clone_hint.png");

    pixmapJumpHint = new QPixmap;
    if (!pixmapJumpHint->load(":/resources/jump_hint.png"))
        qWarning("Failed to load jump_hint.png");

    pixmapOldJumpPieceWhite = new QPixmap;
    if (!pixmapOldJumpPieceWhite->load(":/resources/old_piece_jump_white.png"))
        qWarning("Failed to load old_jump_piece_white.png");

    pixmapOldClonePieceWhite = new QPixmap;
    if (!pixmapOldClonePieceWhite->load(":/resources/old_piece_clone_white.png"))
        qWarning("Failed to load old_clone_piece_white.png");

    pixmapNewPieceWhite = new QPixmap;
    if (!pixmapNewPieceWhite->load(":/resources/new_piece_white.png"))
        qWarning("Failed to load new_piece_white.png");
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

    updateGameGrid();
    judgeWinner();
    setCurrentPlayerHint();

    // show the move AI taken
    char *move = new char[5];
    game.getMoveStr(move);
    unsigned int x0 = move[0] - 'a';
    unsigned int y0 = move[1] - '1';
    unsigned int x1 = move[2] - 'a';
    unsigned int y1 = move[3] - '1';
    for (unsigned int x = 0; x < GameProcess::BOARD_SIZE; x ++)
        for (unsigned int y = 0; y < GameProcess::BOARD_SIZE; y ++)
        {
            QLayoutItem *item = gridLayout->itemAtPosition(y, x);
            auto label = dynamic_cast<ClickableLabel *>(item->widget());

            if (x == x0 && y == y0)
            {
                if (game.getPiece(x0, y0) == GameProcess::Absent)
                    label->setPixmap(*pixmapOldJumpPieceWhite);
                else
                    label->setPixmap(*pixmapOldClonePieceWhite);
            }
            if (x == x1 && y == y1)
                label->setPixmap(*pixmapNewPieceWhite);
        }
}

void MainWindow::createSideBar()
{
    sidebar = new QWidget;
    sidebar->setStyleSheet("background-color: #d7c4bb;");   // Nippon Colors "Haizakura"
    sidebar->setMinimumWidth(200);

    restartButton = new QPushButton(tr("Restart"), sidebar);
    restartButton->setGeometry(QRect(QPoint(50, 50), QSize(100, 50)));
    restartButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    connect(restartButton, &QPushButton::clicked, [this](bool clicked)
    {
        (void)clicked;
        if (!isGameRunning() || (resetMsgBox->exec() == QMessageBox::Ok))
        {
            game = GameProcess(startingPlayer->isChecked());
            resetGame();
            //BotzoneRunner::debug(game, "GAME HAS BEEN RESET");
        }
    });

    exitButton = new QPushButton(tr("Exit"), sidebar);
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
    board->setStyleSheet("background-color: #fad689;");         // Nippon Colors "Usuki"
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
                if (selected != Location(-1, -1))
                {
                    if (std::abs(int(x - selected.first)) <= 1 && std::abs(int(y - selected.second)) <= 1)
                    {
                        label->setPixmap(*pixmapCloneHint);
                        break;
                    }
                    else if (std::abs(int(x - selected.first)) <= 2 && std::abs(int(y - selected.second)) <= 2)
                    {
                        label->setPixmap(*pixmapJumpHint);
                        break;
                    }
                }
                label->setPixmap(*pixmapEmpty);
                break;
            }

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
        // the winner should be judged by the current player's score
        int score = game.getScore();              // when the game is over the current player lost
        bool winner;
        if (score > 0)
            winner = game.player;
        else
            winner = !game.player;
        score = std::abs(score);

        statusBar()->showMessage(tr("The game is over!"));

        addHighScore();

        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setWindowTitle(tr("The game is over!"));

        if (winner)
            msgBox.setText(tr("Black won this round!"));
        else
            msgBox.setText(tr("White won this round!"));

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
    statusBar()->showMessage(QString(game.player ? tr("Black") : tr("White")) + tr("'s turn."));
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

    /*
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
    */
}

// menu entries are created dynamically, dependent on the existing translations
void MainWindow::createLanguageMenu()
{
    langGroup = new QActionGroup(langMenu);
    langGroup->setExclusive(true);

    connect(langGroup, &QActionGroup::triggered, this, &MainWindow::slotLanguageChanged);

    // format system language
    QString defaultLocale = QLocale::system().name();           // e.g. "ja_JP"
    //defaultLocale.truncate(defaultLocale.lastIndexOf('_'));     // e.g. "ja"

    langPath = QApplication::applicationDirPath();
    langPath.append("/languages");
    QDir dir(langPath);
    QStringList fileNames = dir.entryList(QStringList("Translation_*.qm"));

    for (auto i : fileNames)                // "Translation_ja_JP.qm"
    {
        i.truncate(i.lastIndexOf('.'));     // "Translation_ja_JP"
        i.remove(0, i.indexOf('_') + 1);    // "ja_JP"

        QString lang = QLocale::languageToString(QLocale(i).language());
        QAction *action = new QAction(lang, this);
        action->setCheckable(true);
        action->setData(i);

        langMenu->addAction(action);
        langGroup->addAction(action);

        if (defaultLocale == i)
            action->setChecked(true);
    }
}

// called when an enrty in the language menu is called
void MainWindow::slotLanguageChanged(QAction *action)
{
    if (action)
        // load the language according to the action
        loadLanguage(action->data().toString());
}

void MainWindow::switchTranslator(QTranslator &translator, const QString &filename)
{
    // remove the old translator
    qApp->removeTranslator(&translator);

    // load the new translator
    if (translator.load(filename, langPath))
        qApp->installTranslator(&translator);
}

void MainWindow::loadLanguage(const QString &language)
{
    if (currLang != language)
    {
        currLang = language;
        QLocale locale = QLocale(language);
        QLocale::setDefault(locale);
        QString languageName = QLocale::languageToString(locale.language());
        switchTranslator(translator, QString("Translation_%1.qm").arg(language));
        switchTranslator(translatorQt, QString("qt_%1.qm").arg(language));
        statusBar()->showMessage(tr("Current language changed to %1").arg(languageName));
    }
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

    langMenu = menuBar()->addMenu(tr("&Language"));
    createLanguageMenu();

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
        writeSettings();    // always save settings
        if (response == QMessageBox::Yes)
        {
            saveGame();
            model->submitAll();
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
        {
            QMessageBox::warning(this, tr("Game loading failed"), tr("The save data seems to be corrupted. A new game will be created."));
            saveGame();         // clear the corrupted state
        }
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
    record.setValue(as_integer(Columns::startingPlayer), startingPlayer->isChecked() ? tr("Black") : tr("White"));
    // the winner should be judged by the current player's score
    int score = game.getScore();              // when the game is over the current player lost
    bool winner;
    if (score > 0)
        winner = game.player;
    else
        winner = !game.player;
    score = std::abs(score);
    record.setValue(as_integer(Columns::playerWon), winner ? tr("Black") : (whiteAI->isChecked() ? tr("White (AI)") : tr("White")));
    record.setValue(as_integer(Columns::score), score);
    model->insertRecord(-1, record);
}
