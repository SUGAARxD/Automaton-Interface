#pragma once
#include "State.h"
#include "Transition.h"
#include <vector>
#include<fstream>
#include <stack>
#include <QMouseEvent>

class Automaton
{
public:
    Automaton() = default;
    ~Automaton() = default;
    
	virtual bool NodeExists(QMouseEvent* e) = 0;
    virtual bool OtherStateThanSelectedExists(const QPoint& pos) = 0;
    virtual bool TransitionsExists(State* firstState, State* secondState) = 0;
    virtual bool IsDeterministic() const = 0;
    virtual bool HasLambdaTransitions() const = 0;
    virtual bool CheckWord(const QString& word) = 0;

    virtual void SetStates(std::vector<State*> states) = 0;
    virtual void SetAlphabet(std::vector<char> alphabet) = 0;
    virtual void SetTransitions(std::vector<Transition*> transitions) = 0;
    virtual void SetInitialState(State* state) = 0;

    virtual const std::vector<State*>& GetStates() = 0;
    virtual const std::vector<State*>& GetFinalStates() = 0;
    virtual const std::vector<Transition*>& GetTransitions() = 0;
    virtual const std::vector<char>& GetAlphabeth() = 0;
    virtual State* GetInitialState() const = 0;

    virtual int GetContor() = 0;

    virtual void AddState(State* state) = 0;
    virtual void AddState(const QPoint& coordinates, const std::string& name) = 0;
    virtual void AddTransiton(State* state1, State* state2, const QString& character) = 0;
    virtual void AddFinalState(State* state) = 0;
    virtual void RemoveState(State* state) = 0;

    virtual void Reset() = 0;
    
};