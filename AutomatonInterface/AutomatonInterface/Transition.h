#pragma once
#include "State.h"
#include "qstring.h"

class Transition
{
public:
	Transition();
	Transition(State* n1, State* n2, const QString& character);
	Transition(State* n1, State* n2);

	void SetFirstState(State* firstState);
	void SetSecondState(State* secondState);
	void SetCharacter(const QString& character);
	void SetIsVerified(bool b);
	
	State* GetFirstState();
	State* GetSecondState();
	const QString& GetCharacter();
	bool IsVerified();

private:
	State* m_firstState, *m_secondState;
	QString m_character;
	bool isVerified;
};

