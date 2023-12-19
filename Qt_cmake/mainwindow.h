#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qt_tirages.h"
#include "qt_plateau.h"
#include "qt_jetons_bas.h"
#include "qt_jetons_main.h"
#include "../classes/jetons.h"
#include "qt_choicepopup.h"
#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDialog>
#include <QInputDialog>
#include <QLineEdit>
#include <QLCDNumber>
#include <QDesktopServices>
#include <QUrl>

#include "popup_text.h"
#include "qt_popup_yesno.h"
#include "qt_popup_info.h"
#include "qt_popup_tirageoupioche.h"

#define RIEN "../src/rien.png"


class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    QLabel *topScoreLabel;
    QLabel *bottomScoreLabel;
    QLCDNumber *topScoreDisplay;  // Top score display
    QLCDNumber *bottomScoreDisplay;  // Bottom score display

    QPushButton *viewCardsButtonBottom;
    QPushButton *viewJetonsButtonBottom;
    QPushButton *viewReservedCardsButtonBottom;

    QPushButton *viewCardsButtonTop;
    QPushButton *viewJetonsButtonTop;
    QPushButton *viewReservedCardsButtonTop;

    QLabel *topPlayerNameLabel;
    QLabel *bottomPlayerNameLabel;

    QEventLoop wait_for_action_jeton;
    QEventLoop wait_for_action_carte;

    Qt_Plateau* plateau;
    Qt_Tirages* tirages;

    QLabel* quijoue;

    Jeu* jeu;

    struct Handler {
        MainWindow * instance = nullptr;
        ~Handler() {
            delete instance;
            instance = nullptr;
        }
    };

    static Handler handler;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    MainWindow (const MainWindow&) = delete;
    MainWindow& operator=(const MainWindow&) = delete;

    int indice_jeton_click;
    Qt_carte* derniere_carte_click;

public:

    void updateQuiJoue();

    QEventLoop* getCarteWaitLoop() {return &wait_for_action_carte;}
    QEventLoop* getJetonWaitLoop() {return &wait_for_action_jeton;}

    int getIndiceJetonClick() const {return indice_jeton_click;}
    void setIndiceJetonClick(int x) {indice_jeton_click = x;}

    Qt_carte* getDerniereCarteClick() const {return derniere_carte_click;}
    void setDerniereCarteClick(Qt_carte* c) {derniere_carte_click = c;}

    void updateTopScore(int score);  // Méthode de mise à jour du score du haut
    void updateBottomScore(int score);  //  -- du bas

    void updateScores(){
        int s1 = Jeu::getJeu().getCurrentPlayer().getNbPoints();
        int s2 = Jeu::getJeu().getOpponent().getNbPoints();
        topScoreDisplay->display(s1);
        bottomScoreDisplay->display(s2);
    }

    void demanderNoms() {
        InputPopup *popup = new InputPopup(this);

        // Connect signals and slots if needed
        // For example, if you want to do something with the input after it's submitted

        popup->setModal(true); // Set the dialog to be modal
        popup->exec();         // Show the dialog modally

        // Code here will execute after the dialog is closed
        // You can process the input here if needed
    }

    void setTopPlayerName(const QString &name);
    void setBottomPlayerName(const QString &name);

    void updatePlateau();
    void updateTirages();
    void updatePrivileges();

    static MainWindow& getMainWindow(){
        if (handler.instance == nullptr)  handler.instance = new MainWindow();
        return *handler.instance;
    }

    void freeMainWindow(){
        delete handler.instance;
        handler.instance = nullptr;
    }

    void deactivateButtons();
    void activateJetons();
    void activateForReserve();

    Qt_Tirages* getTirages() const {return tirages;}


private slots:
    void showBoughtCardsTop();
    void showReservedCardsTop();
    void showJetonsTop();

    void showBoughtCardsBottom();
    void showReservedCardsBottom();
    void showJetonsBottom();


    void remplirPlateau();
    void openWebLink();
    void nextAction(int* tmp, Joueur* j);
    void YesNo(char* choice, const std::string& string);
    void showInfo(const string& string);
    void popTiragePioche(string* choice) {
        popupTiragePioche dialog(this);
        if (dialog.exec() == QDialog::Accepted) {
            *choice = dialog.getUserChoice();
        }
    }

public slots:
    void jetonClicked(Qt_jeton*);
    void carteClicked(Qt_carte*);


signals:
    void triggerNextAction(int* tmp, Joueur* j);
    void triggerYesNo(char* choice, const std::string& string = "");
    void triggerInfo(const string& string);
    void triggerTiragePioche(string* choice);
    void jetonActionDone();
    void carteActionDone();

};

#endif // MAINWINDOW_H
