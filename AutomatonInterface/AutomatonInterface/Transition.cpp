#include "Transition.h"

Transition::Transition()
	:m_firstState{nullptr},m_secondState{nullptr}, m_character{' '}, m_pdCharacter{' '},isVerified{false}
{
}

Transition::Transition(State* n1, State* n2, const QString& character, const QString& pdCharacter)
	:m_firstState{ n1 }, m_secondState{ n2 }, m_character{ character }, m_pdCharacter{ pdCharacter },isVerified{false}
{
}

Transition::Transition(State* n1, State* n2)
	:m_firstState{n1},m_secondState{n2}, m_character{' '},isVerified{false}
{
}

void Transition::SetFirstState(State* firstState)
{
	m_firstState = firstState;
}

void Transition::SetSecondState(State* secondState)
{
	m_secondState = secondState;
}

void Transition::SetCharacter(const QString& character)
{
	m_character = character;
}

void Transition::SetPDCharacter(const QString& pdCharacter)
{
	m_pdCharacter = pdCharacter;
}

void Transition::SetIsVerified(bool b)
{
	isVerified = b;
}

State* Transition::GetFirstState()
{
	return m_firstState;
}

State* Transition::GetSecondState()
{
	return m_secondState;
}

QString Transition::GetCharacter()
{
	return m_character;
}

QString Transition::GetPDCharacter()
{
	return m_pdCharacter;
}

bool Transition::IsVerified()
{
	return isVerified;
}
