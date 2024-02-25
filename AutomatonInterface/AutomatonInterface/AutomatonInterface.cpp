#include "AutomatonInterface.h"

AutomatonInterface::AutomatonInterface(QWidget* parent)
    : QMainWindow(parent)
    , m_firstState{ nullptr }
    , m_secondState{ nullptr }
    , m_isMousePressed{ false }
    , m_isDrawProccesActive{ false }
    , currentState{ nullptr }
    , index{ 0 }
{
    ui.setupUi(this);
    this->setFixedSize(1200, 700);

    m_reset = new QPushButton("Reset Automaton", this);
    m_reset->setFixedSize(100, 30);
    m_reset->move(this->rect().width() - 100, 60);
    connect(m_reset, &QPushButton::clicked, this,
        [this]()
        {
            m_automaton->Reset();
            ResetCheckWordProcess();
            update();
        }
    );
    m_checkWord = new QPushButton("Check Word", this);
    m_checkWord->setFixedSize(100, 30);
    m_checkWord->move(this->rect().width() - 100, 90);
    connect(m_checkWord, &QPushButton::clicked, this, &AutomatonInterface::CheckWord);

    m_DFA.SetIsDeterministic(true);
    m_NFALambda.SetHasLambdaTransitions(true);

    m_automaton = &m_NFA;

    m_radioButtonAFN = new QRadioButton("NFA", this);
    m_radioButtonAFN->setFixedSize(80, 20);
    m_radioButtonAFN->move(this->rect().width() - 100, 0);
    connect(m_radioButtonAFN, &QRadioButton::clicked, this,
        [this]()
        {
            m_lambdaTransition->setVisible(false);
            m_automaton = &m_NFA;
            m_firstState = nullptr;
            m_secondState = nullptr;
            ResetCheckWordProcess();
            update();
        }
    );

    m_radioButtonAFD = new QRadioButton("DFA", this);
    m_radioButtonAFD->setFixedSize(80, 20);
    m_radioButtonAFD->move(this->rect().width() - 100, 20);
    connect(m_radioButtonAFD, &QRadioButton::clicked, this,
        [this]()
        {
            m_lambdaTransition->setVisible(false);
            m_automaton = &m_DFA;
            m_firstState = nullptr;
            m_secondState = nullptr;
            ResetCheckWordProcess();
            update();
        }
    );

    m_radioButtonAFNLambda = new QRadioButton("NFA λ", this);
    m_radioButtonAFNLambda->setFixedSize(80, 20);
    m_radioButtonAFNLambda->move(this->rect().width() - 100, 40);
    connect(m_radioButtonAFNLambda, &QRadioButton::clicked, this,
        [this]()
        {
            m_lambdaTransition->setVisible(true);
            m_automaton = &m_NFALambda;
            m_firstState = nullptr;
            m_secondState = nullptr;
            ResetCheckWordProcess();
            update();
        }
    );

    m_addState = new QRadioButton("Add State", this);
    m_addState->setFixedSize(100, 20);
    m_addState->move(1, 0);
    m_addState->setChecked(true);

    m_removeState = new QRadioButton("Remove State", this);
    m_removeState->setFixedSize(100, 20);
    m_removeState->move(1, 20);

    m_selectInitialState = new QRadioButton("Initial State", this);
    m_selectInitialState->setFixedSize(100, 20);
    m_selectInitialState->move(1, 40);

    m_selectFinalState = new QRadioButton("Final State", this);
    m_selectFinalState->setFixedSize(100, 20);
    m_selectFinalState->move(1, 60);

    m_lambdaTransition = new QRadioButton("λ Transition", this);
    m_lambdaTransition->setFixedSize(100, 20);
    m_lambdaTransition->move(1, 80);
    m_lambdaTransition->setVisible(false);

    m_group1.addButton(m_selectInitialState);
    m_group1.addButton(m_selectFinalState);
    m_group1.addButton(m_lambdaTransition);
    m_group1.addButton(m_addState);
    m_group1.addButton(m_removeState);
    m_group1.addButton(m_checkWord);

    m_group2.addButton(m_radioButtonAFN);
    m_group2.addButton(m_radioButtonAFD);
    m_group2.addButton(m_radioButtonAFNLambda);

    m_radioButtonAFN->setChecked(true);

    m_timer = new QTimer(this);
    m_timer->setInterval(800);

    connect(m_timer, &QTimer::timeout, this, &AutomatonInterface::DrawCheckWordProcess);

    m_textbox = new QLineEdit(this);
    m_textbox->setVisible(false);
    m_textbox->setReadOnly(true);
    m_textbox->move(this->width() - 500, 0);

    m_checkWordsFromFile = new QPushButton("Check File Words", this);
    m_checkWordsFromFile->setFixedSize(100, 30);
    m_checkWordsFromFile->move(this->rect().width() - 100, 120);
    connect(m_checkWordsFromFile, &QPushButton::clicked, this,
        [this]()
        {
            std::vector <std::pair<QString, QString>> words;
            std::ifstream fin("words.txt");
            while (!fin.eof())
            {
                std::string word;
                std::getline(fin, word);
                QString qstringWord(QString::fromStdString(word));
                if (m_automaton->CheckWord(qstringWord))
                {
                    words.push_back({ qstringWord , " - ACCEPTED!" });
                }
                else
                {
                    words.push_back({ qstringWord , " - REJECTED!" });
                }
            }
            fin.close();
            CheckWordsWindow* cwWindow = new CheckWordsWindow(words, this);
            cwWindow->show();
            update();
        }
    );
}

AutomatonInterface::~AutomatonInterface()
{
    delete m_radioButtonAFD;
    delete m_radioButtonAFN;
    delete m_selectInitialState;
    delete m_selectFinalState;
    delete m_reset;
    delete m_lambdaTransition;
    delete m_addState;
    delete m_removeState;
    delete m_checkWord;
    delete m_checkWordsFromFile;
    delete m_timer;
    delete m_textbox;
}

void AutomatonInterface::mousePressEvent(QMouseEvent* e)
{
    if (e->button() == Qt::RightButton)
    {
        if (!m_automaton->NodeExists(e))
        {
            if (m_addState->isChecked())
            {
                m_automaton->AddState(e->pos(), "q" + std::to_string(m_automaton->GetContor()));
            }
        }
        else
        {
            for (State* state : m_automaton->GetStates())
            {
                if (m_selectInitialState->isChecked())
                {
                    state->SetInitial(false);
                }
                if (fabs(state->GetX() - e->pos().x()) < State::kDiameter / 2.0 &&
                    fabs(state->GetY() - e->pos().y()) < State::kDiameter / 2.0)
                {
                    if (m_selectInitialState->isChecked())
                    {
                        state->SetInitial(true);
                        m_automaton->SetInitialState(state);
                    }
                    else if (m_selectFinalState->isChecked())
                    {
                        state->SetFinal(!state->IsFinal());
                        m_automaton->AddFinalState(state);
                    }
                    else if (m_removeState->isChecked())
                    {
                        m_automaton->RemoveState(state);
                    }
                    else
                    {
                        if (m_firstState == nullptr)
                        {
                            m_firstState = state;
                            m_firstState->Select();
                        }
                        else if (m_firstState->IsSelected())
                        {
                            m_secondState = state;
                            if (!m_automaton->TransitionsExists(m_firstState, m_secondState))
                            {
                                if (m_lambdaTransition->isChecked() && m_lambdaTransition->isVisible())
                                {
                                    m_automaton->AddTransiton(m_firstState, m_secondState, m_NFA.LAMBDA);
                                }
                                else
                                {
                                    bool ok;
                                    QString text = QInputDialog::getText(nullptr, "Get Char", "Enter a character:", QLineEdit::Normal, "", &ok);
                                    if (!text.isEmpty())
                                    {
                                        m_automaton->AddTransiton(m_firstState, m_secondState, text);
                                    }
                                }
                            }
                            m_firstState->Deselect();
                            m_firstState = nullptr;
                            m_secondState = nullptr;
                        }
                        else
                        {
                            m_firstState = nullptr;
                        }
                    }
                }
            }
        }
    }
    else if (e->button() == Qt::LeftButton)
    {
        for (State* state : m_automaton->GetStates())
        {
            state->Deselect();
            if (fabs(state->GetX() - e->pos().x()) < State::kDiameter / 2.0 &&
                fabs(state->GetY() - e->pos().y()) < State::kDiameter / 2.0)
            {
                state->Select();
                m_isMousePressed = true;
                break;
            }
        }
    }

    update();
}

void AutomatonInterface::mouseMoveEvent(QMouseEvent* e)
{
    if (m_isMousePressed)
        if (Qt::LeftButton)
        {
            for (State* state : m_automaton->GetStates())
                if (state->IsSelected())
                {
                    if (!m_automaton->OtherStateThanSelectedExists(e->pos()))
                        state->SetCoordinates(e->pos());
                }
        }
    update();
}

void AutomatonInterface::mouseReleaseEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton)
    {
        m_isMousePressed = false;
        for (State* state : m_automaton->GetStates())
        {
            state->Deselect();
        }
    }
    update();
}

void AutomatonInterface::PaintState(QPainter& painter, State* state)
{
    if (state->IsSelected())
        painter.setPen(Qt::red);
    else
        painter.setPen(Qt::black);
    painter.setBrush(state->GetColor());

    if (state->IsFinal())
    {
        QRect rect(state->GetX() - (State::kDiameter + 6) / 2, state->GetY() - (State::kDiameter + 6) / 2, State::kDiameter + 6, State::kDiameter + 6);
        painter.drawEllipse(rect);
    }

    QRect rect(state->GetX() - State::kDiameter / 2, state->GetY() - State::kDiameter / 2, State::kDiameter, State::kDiameter);
    painter.drawEllipse(rect);
    QString label = QString::fromStdString(state->GetStateName());
    painter.drawText(rect, Qt::AlignCenter, label);

    if (state->IsInitial())
    {
        State* tempState = new State;
        tempState->SetCoordinates(QPoint(state->GetX() - 30, state->GetY() - 60));
        DrawLine(tempState, state, painter, "");
        delete tempState;
    }
    painter.setBrush(Qt::NoBrush);
}

void AutomatonInterface::DrawLine(State* state1, State* state2, QPainter& painter, const QString& c)
{
    painter.setPen(Qt::black);

    if (state1 == state2)
    {
        painter.setBrush(Qt::NoBrush);
        QRect rect((state1->GetX() - State::kDiameter / 2) + 10, (state1->GetY() - State::kDiameter / 2) - 10, State::kDiameter - 5, State::kDiameter - 5);
        painter.drawEllipse(rect);
        painter.setPen(Qt::NoPen);
        QRect r((state1->GetX() - State::kDiameter / 2) + 32, (state1->GetY() - State::kDiameter / 2) - 21, 10 + (c.size() + 1) * 5, 20);
        painter.drawRect(r);
        painter.setPen(Qt::black);
        QString label(c);
        painter.drawText(r, Qt::AlignCenter, label);
        return;
    }

    int radius = State::kDiameter / 2;
    qreal dx = state2->GetX() - state1->GetX();
    qreal dy = state2->GetY() - state1->GetY();
    qreal distance = qSqrt(dx * dx + dy * dy);
    qreal x1_intersec = state1->GetX() + (dx * radius / distance);
    qreal y1_intersec = state1->GetY() + (dy * radius / distance);
    qreal x2_intersec = state2->GetX() - (dx * radius / distance);
    qreal y2_intersec = state2->GetY() - (dy * radius / distance);

    painter.drawLine(x1_intersec, y1_intersec, x2_intersec, y2_intersec);
    DrawArrow(painter, QPoint(x1_intersec, y1_intersec), QPoint(x2_intersec, y2_intersec));
}

void AutomatonInterface::DrawArrow(QPainter& painter, QPoint Q1, QPoint Q2)
{
    qreal arrowSize = State::kDiameter / 3.0;
    painter.setBrush(QBrush(Qt::black));
    QLineF line(Q2, Q1);

    double angle = std::atan2(-line.dy(), line.dx());
    QPointF arrowP1 = line.p1() + QPointF(sin(angle + M_PI / 3) * arrowSize, cos(angle + M_PI / 3) * arrowSize);
    QPointF arrowP2 = line.p1() + QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize, cos(angle + M_PI - M_PI / 3) * arrowSize);

    QPolygonF arrowHead;
    arrowHead.clear();
    arrowHead << line.p1() << arrowP1 << arrowP2;
    painter.drawPolygon(arrowHead);

    painter.setBrush(QBrush(Qt::white));
}

void AutomatonInterface::DrawBox(State* state1, State* state2, QPainter& painter, const QString& c)
{
    if (state1 == state2)
        return;
    painter.setBrush(Qt::white);
    painter.setPen(Qt::black);
    QLineF line(state1->GetCoordinates(), state2->GetCoordinates());
    double angle = std::atan2(-line.dy(), line.dx());
    QPoint middlePoint = (state1->GetCoordinates() + state2->GetCoordinates()) / 2;
    QRect r(middlePoint.x() + (line.length() / 2 - State::kDiameter * 1.3) * std::cos(angle) - (c.size() + 1) * 2.5, middlePoint.y() - (line.length() / 2 - State::kDiameter * 1.3) * std::sin(angle) - 10, (c.size() + 1) * 5, 20);
    painter.drawRect(r);
    QString label(c);
    painter.drawText(r, Qt::AlignCenter, label);
    painter.setBrush(Qt::NoBrush);
}

void AutomatonInterface::ResetCheckWordProcess()
{
    m_timer->stop();
    for (State* state : m_automaton->GetStates())
        state->SetColor(Qt::white);
    m_textbox->setPlaceholderText("");
    m_textbox->setVisible(false);
    m_isDrawProccesActive = false;
}

void AutomatonInterface::CheckWord()
{
    if (!m_isDrawProccesActive)
    {
        if (m_automaton->GetInitialState() != nullptr && m_automaton->GetFinalStates().size() > 0)
        {
            index = 0;
            std::stack<std::pair<State*, int>>().swap(currentStates);
            currentStates.push({ m_automaton->GetInitialState(), index });
            m_isDrawProccesActive = true;
            bool ok;
            word = QInputDialog::getText(nullptr, "Check Word", "Enter a word:", QLineEdit::Normal, "", &ok);
            if (word.isEmpty())
                word = m_DFA.LAMBDA;
            m_textbox->setVisible(true);
            m_textbox->setStyleSheet("background-color: transparent; color: black; border: none; font-size: 24px;");
            QFontMetrics qfm(m_textbox->font());
            m_textbox->setFixedWidth(qfm.horizontalAdvance(word) + 300);
            m_textbox->setFixedHeight(50);
            m_timer->start();
        }
    }
    else
    {
        ResetCheckWordProcess();
    }
    update();
}

void AutomatonInterface::DrawCheckWordProcess()
{
    if (!currentStates.empty())
    {
        index = currentStates.top().second;
    }
    else if (index < word.size())
        index++;

    QString label;
    for (int i = 0; i < index; i++)
    {
        label += word[i];
    }
    m_textbox->setPlaceholderText(label);

    if (!currentStates.empty())
    {
        currentState = currentStates.top().first;
        currentStates.pop();
        currentState->SetColor("#bdf2e8");

        if (index < word.size())
        {
            if (word == m_DFA.LAMBDA && currentState->IsFinal())
            {
                currentState->SetColor("#01ff71");
                m_textbox->setStyleSheet("background-color: transparent; color: green; border: none; font-size: 24px;");
                m_textbox->setPlaceholderText(label + " - ACCEPTED!");
                m_timer->stop();
                update();
                return;
            }
            for (Transition* transition : m_automaton->GetTransitions())
            {
                if (transition->GetFirstState() == currentState && (transition->GetCharacter() == word[index] || transition->GetCharacter() == m_DFA.LAMBDA))
                {
                    if (transition->GetCharacter() == m_NFALambda.LAMBDA)
                        currentStates.push({ transition->GetSecondState(), index });
                    else
                        currentStates.push({ transition->GetSecondState(), index + 1 });
                }
            }
        }
        else
        {
            if (currentState->IsFinal())
            {
                currentState->SetColor("#01ff71");
                m_textbox->setStyleSheet("background-color: transparent; color: green; border: none; font-size: 24px;");
                m_textbox->setPlaceholderText(label + " - ACCEPTED!");
                m_timer->stop();
            }
        }
    }
    else
    {
        currentState->SetColor(Qt::gray);
        m_textbox->setStyleSheet("background-color: transparent; color: red; border: none; font-size: 24px;");
        m_textbox->setPlaceholderText(label + " - REJECTED!");
        m_timer->stop();
    }
    update();
}

void AutomatonInterface::paintEvent(QPaintEvent* e)
{
    QPainter p(this);

    std::vector<std::tuple<State*, State*, QString>> transitions;

    for (const auto& transition : m_automaton->GetTransitions())
    {
        int indexTransition = -1;
        for (int index = 0; index < transitions.size(); index++)
        {
            if (std::get<0>(transitions[index]) == transition->GetFirstState() && std::get<1>(transitions[index]) == transition->GetSecondState())
                indexTransition = index;
        }

        if (indexTransition == -1)
        {
            transitions.push_back(std::make_tuple(transition->GetFirstState(), transition->GetSecondState(), transition->GetCharacter()));
        }
        else
        {
            transitions[indexTransition] = std::make_tuple(transition->GetFirstState(), transition->GetSecondState(), std::get<2>(transitions[indexTransition]) + " " + transition->GetCharacter());
        }
    }

    for (const auto& transition : transitions)
    {
        p.setBrush(Qt::white);
        DrawLine(std::get<0>(transition), std::get<1>(transition), p, std::get<2>(transition));
    }
    for (const auto& state : m_automaton->GetStates())
    {
        p.setBrush(state->GetColor());
        PaintState(p, state);
    }
    for (const auto& transition : transitions)
    {
        p.setBrush(Qt::white);
        DrawBox(std::get<0>(transition), std::get<1>(transition), p, std::get<2>(transition));
    }

    update();
}