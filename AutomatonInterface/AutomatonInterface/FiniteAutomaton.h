﻿#pragma once
#include "Automaton.h"

class FiniteAutomaton : public Automaton
{
public:
	const QString LAMBDA = "λ";

public:
	FiniteAutomaton();
	~FiniteAutomaton();

	bool NodeExists(QMouseEvent* e) override;
	bool OtherStateThanSelectedExists(const QPoint& pos) override;
	bool TransitionsExists(State* firstState, State* secondState) override;
	bool IsDeterministic() const override;
	bool HasLambdaTransitions() const override;
	bool CheckWord(const QString& word);

	void SetStates(std::vector<State*> states) override;
	void SetAlphabet(std::vector<char> alphabet) override;
	void SetTransitions(std::vector<Transition*> transitions) override;
	void SetInitialState(State* state) override;
	void SetIsDeterministic(bool isDeterministic);
	void SetHasLambdaTransitions(bool hasLambdaTransitions);
	

	const std::vector<State*>& GetStates() override;
	const std::vector<State*>& GetFinalStates() override;
	const std::vector<Transition*>& GetTransitions() override;
	const std::vector<char>& GetAlphabeth() override;
	State* GetInitialState() const override;

	int GetContor() override;

	void AddState(State* state) override;
	void AddState(const QPoint& coordinates, const std::string& name) override;
	void AddTransiton(State* state1, State* state2, const QString& character) override;
	void AddFinalState(State* state) override;
	void RemoveState(State* state) override;

	void Reset() override;

private:
	std::vector<State*> m_states;
	std::vector<char> m_alphabet;
	std::vector<Transition*> m_transitions;
	State* m_initialState;
	std::vector<State*> m_finalStates;
	bool m_isDeterministic;
	bool m_hasLambdaTransitions;
	int m_contor;
};