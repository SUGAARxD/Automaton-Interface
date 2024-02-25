#pragma once
#include <string>
#include <qpoint.h>
#include <qcolor.h>
class State
{
public:
	State();
	State(const std::string& stateName);
	State(State* node);

	void SetCoordinates(const QPoint& coordinates);
	void SetStateName(const std::string& stateName);
	void Select();
	void Deselect();
	void SetFinal(bool isFinal);
	void SetInitial(bool isInitial);
	void SetColor(const QColor& color);

	const std::string& GetStateName();
	bool IsSelected();
	bool IsFinal();
	bool IsInitial();
	int GetX();
	int GetY();
	const QPoint& GetCoordinates();
	const QColor& GetColor();

public:
	static const size_t kDiameter{ 40 };

private:
	QPoint m_coordinates;
	std::string m_stateName;
	bool m_isSelected;
	bool m_isFinal;
	bool m_isInitial;
	QColor m_color;
};

