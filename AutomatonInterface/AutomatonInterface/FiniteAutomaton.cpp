#include "FiniteAutomaton.h"


FiniteAutomaton::FiniteAutomaton() : m_initialState{ nullptr }, m_isDeterministic{ false }, m_hasLambdaTransitions{ false }, m_contor { 0 }
{
}

FiniteAutomaton::~FiniteAutomaton()
{
    Reset();
}

bool FiniteAutomaton::NodeExists(QMouseEvent *e)
{
    for (State* n : m_states)
    {
            if (fabs(n->GetX() - e->pos().x()) < State::kDiameter * 2.5 &&
                fabs(n->GetY() - e->pos().y()) < State::kDiameter * 2.5)
                return true;
    }
    return false;
}

bool FiniteAutomaton::OtherStateThanSelectedExists(const QPoint& pos)
{
    for (State* state : m_states)
    {
        if (!state->IsSelected())
            if (fabs(state->GetX() - pos.x()) < State::kDiameter * 2.5 &&
                fabs(state->GetY() - pos.y()) < State::kDiameter * 2.5)
            {
                return true;
            }
    }
    return false;
}

bool FiniteAutomaton::TransitionsExists(State* firstState, State* secondState)
{
    bool ok = false;
    for (int index = 0; index < m_transitions.size(); index++)
    {
        Transition* transition = m_transitions[index];
        if (transition->GetFirstState() == firstState && transition->GetSecondState() == secondState)
        {
            auto itFirst = std::find(m_transitions.begin(), m_transitions.end(), transition);
            m_transitions.erase(itFirst);
            delete transition;
            index--;
            ok = true;
        }
    }
    return ok;
}

bool FiniteAutomaton::IsDeterministic() const
{
    return m_isDeterministic;
}

bool FiniteAutomaton::HasLambdaTransitions() const
{
    return m_hasLambdaTransitions;
}

bool FiniteAutomaton::CheckWord(const QString& word)
{
    std::stack<std::pair<State*, int>> currentStates;
    if (m_initialState != nullptr)
        currentStates.push({ m_initialState, 0 });
    while (!currentStates.empty())
    {
        auto [currentState, index] = currentStates.top();
        currentStates.pop();

        if (index < word.size())
        {
            if (word == LAMBDA && currentState->IsFinal())
            {
                return true;
            }
            for (Transition* transition : m_transitions)
            {
                if (transition->GetFirstState() == currentState && (transition->GetCharacter() == word[index] || transition->GetCharacter() == LAMBDA))
                {
                    if (transition->GetCharacter() == LAMBDA)
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
                return true;
            }
        }
    }
    return false;
}


void FiniteAutomaton::SetStates(std::vector<State*> states)
{
	m_states = states;
}

void FiniteAutomaton::SetAlphabet(std::vector<char> alphabet)
{
	m_alphabet = alphabet;
}

void FiniteAutomaton::SetTransitions(std::vector<Transition*> transitions)
{
	m_transitions = transitions;
}

void FiniteAutomaton::SetInitialState(State* state)
{
    m_initialState = state;
}

void FiniteAutomaton::SetIsDeterministic(bool isDeterministic)
{
    m_isDeterministic = isDeterministic;
}

void FiniteAutomaton::SetHasLambdaTransitions(bool hasLambdaTransitions)
{
    m_hasLambdaTransitions = hasLambdaTransitions;
}

std::vector<State*> FiniteAutomaton::GetStates()
{
	return m_states;
}

std::vector<Transition*> FiniteAutomaton::GetTransitions()
{
    return m_transitions;
}

std::vector<char> FiniteAutomaton::GetAlphabeth()
{
    return m_alphabet;
}

State* FiniteAutomaton::GetInitialState() const
{
    return m_initialState;
}

std::vector<State*> FiniteAutomaton::GetFinalStates()
{
    return m_finalStates;
}

int FiniteAutomaton::GetContor()
{
    return m_contor++;
}

void FiniteAutomaton::AddState(State* state)
{
    m_states.push_back(state);
}

void FiniteAutomaton::AddState(const QPoint& p, const std::string& name)
{
    State* state = new State;
    state->SetCoordinates(p);
    state->SetStateName(name);
    m_states.push_back(state);
}

void FiniteAutomaton::AddTransiton(State* state1, State* state2, const QString& character)
{
    std::vector<QChar> characters;

    for (int index = 0; index < character.size(); index++)
    {
        if (index < character.size() - 1)
        {
            if (character[index] != ' ' && character[index + 1] != ' ')
                return;
        }
        if (character[index] != ' ')
            if (std::find(characters.begin(), characters.end(), character[index]) != characters.end())
                return;
            else
            {
                if (m_isDeterministic)
                {
                    for (const auto& transition : m_transitions)
                        if (transition->GetFirstState() == state1 && transition->GetSecondState() != state2 && transition->GetCharacter() == character[index])
                            return;
                }
                characters.push_back(character[index]);
            }
    }

    for (QChar chr : characters)
    {
        Transition* transition = new Transition(state1, state2);
        transition->SetCharacter(chr);
        m_transitions.push_back(transition);
        if(std::find(m_alphabet.begin(),m_alphabet.end(),chr)==m_alphabet.end())
            if(chr!=this->LAMBDA)
                m_alphabet.push_back(chr.toLatin1());
    }
}

void FiniteAutomaton::AddFinalState(State* state)
{
    if (state->IsFinal())
    {
        m_finalStates.push_back(state);
    }
    else
    {
        auto it = std::find(m_finalStates.begin(), m_finalStates.end(), state);
        if (it != m_finalStates.end())
            m_finalStates.erase(it);
    }
}

void FiniteAutomaton::RemoveState(State* state)
{
    for (int index = 0; index < m_transitions.size();)
    {
        Transition* transition = m_transitions[index];
        if (transition->GetFirstState() == state || transition->GetSecondState() == state)
        {
            auto it = std::find(m_transitions.begin(), m_transitions.end(), transition);
            m_transitions.erase(it);
            delete transition;
        }
        else
        {
            index++;
        }
    }
    auto it = std::find(m_states.begin(), m_states.end(), state);
    m_states.erase(it);
    if (state->IsInitial())
        m_initialState = nullptr;
    if (state->IsFinal())
    {
        auto it = std::find(m_finalStates.begin(), m_finalStates.end(), state);
        m_finalStates.erase(it);
    }
    delete state;
}

void FiniteAutomaton::Reset()
{
    for (Transition* transition : m_transitions)
        if (transition)
            delete transition;
    for (State* state : m_states)
        if (state)
            delete state;
    m_contor = 0;
    m_transitions.clear();
    m_states.clear();
}

std::ofstream& operator<<(std::ofstream& ofs, const FiniteAutomaton& other)
{
    if (other.HasLambdaTransitions())
    {
        ofs << "AFN lambda"<<"\n";
    }
    else if (other.IsDeterministic())
    {
        ofs << "AFD" << "\n";
    }
    else 
    {
        ofs << "AFN" << "\n";
    }
    ofs << other.m_states.size()<<"\n";
    for (const auto& x : other.m_states)
    {
        ofs << x->GetName() << "\n";
        ofs << x->GetX() << "\n";
        ofs << x->GetY() << "\n";
    }
    ofs << other.m_alphabet.size()<<"\n";
    for (const auto& x : other.m_alphabet)
    {
        ofs << x << "\n";
    }
    ofs << other.m_transitions.size()<<"\n";
    for (Transition* x : other.m_transitions)
    {
        ofs << x->GetFirstState()->GetName() << "\n";
        ofs << x->GetCharacter().toStdString() << "\n";
        ofs << x->GetSecondState()->GetName() << "\n";
    }
    ofs << other.m_initialState->GetName()<<"\n";
    ofs << other.m_finalStates.size()<<"\n";
    for (State* s : other.m_finalStates)
    {
        ofs << s->GetName() << "\n";
    }
    return ofs;
}

std::ifstream& operator>>(std::ifstream& ifs, FiniteAutomaton& other)
{
    
    int cont;
    ifs >> cont;
    for (int i = 0; i < cont; i++)
    {
        std::string nume;
        int x, y;
        ifs >> nume>>x>>y;
        QPoint coord(x,y);
        State* state=new State;
        state->SetStateName(nume);
        state->SetCoordinates(coord);
        other.AddState(state);
    }
    ifs >> cont;
    std::vector<char> alphabet;
    for (int i = 0; i < cont; i++)
    {
        char c;
        ifs >> c;
        alphabet.push_back(c);
    }
    other.SetAlphabet(alphabet);
    ifs >> cont;
    std::vector<Transition*> transitions;
    for (int i = 0; i < cont; i++)
    {
        Transition* transition=new Transition;
        std::string nume1, nume2, character;
        ifs >> nume1 >> character >> nume2;
        for (State* s : other.GetStates())
        {
            if (s->GetName() == nume1)
            {
                transition->SetFirstState(s);
            }
            if (s->GetName() == nume2)
            {
                transition->SetSecondState(s);
            }
        }
        transition->SetCharacter(QString::fromStdString(character));
        transitions.push_back(transition);
    }
    other.SetTransitions(transitions);
    std::string numeInit;
    ifs >> numeInit;
    for (State* s : other.GetStates())
    {
        if (s->GetStateName() == numeInit)
        {
            s->SetInitial(true);
            other.SetInitialState(s);
        }
    }
    ifs >> cont;
    for (int i = 0; i < cont; i++)
    {
        std::string finalState;
        ifs >> finalState;
        for (State* s : other.GetStates())
        {
            if (s->GetName() == finalState)
            {
                s->SetFinal(true);
                other.AddFinalState(s);
            }
        }
    }
    other.m_contor = other.GetStates().size();
    return ifs;
}
