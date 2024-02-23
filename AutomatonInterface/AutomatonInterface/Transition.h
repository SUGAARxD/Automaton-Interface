#pragma once
#include "State.h"
#include "qstring.h"

class Transition
{
public:
	Transition();
	Transition(State* n1, State* n2, const QString& character, const QString& pdCharacter = " ");
	Transition(State* n1, State* n2);

	void SetFirstState(State* firstState);
	void SetSecondState(State* secondState);
	void SetCharacter(const QString& character);
	void SetPDCharacter(const QString& pdCharacter);
	void SetIsVerified(bool b);
	
	State* GetFirstState();
	State* GetSecondState();
	QString GetCharacter();
	QString GetPDCharacter();
	bool IsVerified();

private:
	State* m_firstState, *m_secondState;
	QString m_character;
	QString m_pdCharacter;
	bool isVerified;
};

