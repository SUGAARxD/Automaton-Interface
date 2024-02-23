#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_AutomatonInterface.h"
#include "FiniteAutomaton.h"
#include <QMainWindow>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QInputDialog>
#include <qpainter.h>
#include <qpushbutton.h>
#include <QRadioButton.h>
#include <QButtonGroup.h>
#include <QTimer.h>
#include <QLineEdit>
#include <QFontMetrics>
#include <stack>
#include "CheckWordsWindow.h"

class AutomatonInterface : public QMainWindow
{
    Q_OBJECT

public:

    AutomatonInterface(QWidget* parent = nullptr);
    ~AutomatonInterface();

    void PaintState(QPainter& painter, State* state);
    void DrawLine(State* state1, State* state2, QPainter& painter, const QString& c);
    void DrawArrow(QPainter& painter, QPoint Q1, QPoint Q2);
    void DrawBox(State* state1, State* state2, QPainter& painter, const QString& c);
    void ResetCheckWordProcess();

private slots:
    void CheckWord();
    void DrawCheckWordProcess();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    Ui::AutomatonInterfaceClass ui;
    bool m_isMousePressed;
    bool m_isDrawProccesActive;

    State* m_firstState, * m_secondState;
    Automaton* m_automaton;
    FiniteAutomaton m_AFD;
    FiniteAutomaton m_AFN;
    FiniteAutomaton m_AFNLambda;

    QRadioButton* m_radioButtonAFN, * m_radioButtonAFD, * m_radioButtonAFNLambda;
    QRadioButton* m_selectInitialState;
    QRadioButton* m_selectFinalState;
    QRadioButton* m_lambdaTransition;
    QRadioButton* m_addState;
    QRadioButton* m_removeState;
    QPushButton* m_reset;
    QPushButton* m_checkWord;
    QPushButton* m_checkWordsFromFile;

    QButtonGroup m_group1;
    QButtonGroup m_group2;

    QTimer* m_timer;
    QLineEdit* m_textbox;

    std::stack<std::pair<State*, int>> currentStates;
    int index;
    State* currentState;
    QString word;
};
