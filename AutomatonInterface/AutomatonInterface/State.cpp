#include "State.h"

State::State() :
	m_coordinates{ QPoint(0,0) }, m_isFinal{ false }, m_isInitial{ false }, m_isSelected{ false }, m_stateName{ "" }, m_color{ Qt::white }
{
}

State::State(const std::string& nodeName)
	:m_stateName{ nodeName }, m_isSelected{ false }, m_isInitial{ false }, m_isFinal{ false }, m_color{ Qt::white }
{
}

State::State(State* state)
	:m_stateName{ state->m_stateName }, m_coordinates{ state->m_coordinates }, m_isSelected{ false }, m_isInitial{ false }, m_isFinal{ false }, m_color{ Qt::white }
{
}

void State::SetStateName(const std::string& stateName)
{
	m_stateName = stateName;
}

const std::string& State::GetStateName()
{
	return m_stateName;
}

void State::Select()
{
	m_isSelected = true;
}

void State::Deselect()
{
	m_isSelected = false;
}

void State::SetFinal(bool isFinal)
{
	m_isFinal = isFinal;
}

void State::SetInitial(bool isInitial)
{
	m_isInitial = isInitial;
}

void State::SetColor(const QColor& color)
{
	m_color = color;
}

bool State::IsSelected()
{
	return m_isSelected;
}

bool State::IsFinal()
{
	return m_isFinal;
}

bool State::IsInitial()
{
	return m_isInitial;
}

void State::SetCoordinates(const QPoint& coordinates)
{
	m_coordinates = coordinates;
}

int State::GetX()
{
	return m_coordinates.x();
}

int State::GetY()
{
	return m_coordinates.y();
}

const QPoint& State::GetCoordinates()
{
	return m_coordinates;
}

const QColor& State::GetColor()
{
	return m_color;
}