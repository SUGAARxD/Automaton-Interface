#pragma once
#include <QMainWindow>
#include <QString>
#include <QLabel>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QScrollBar>

class CheckWordsWindow : public QMainWindow
{
	Q_OBJECT

public:
	CheckWordsWindow(const std::vector<std::pair<QString, QString>>& words, QWidget* parent = nullptr);
	~CheckWordsWindow();

private:
	std::vector<std::pair<QString, QString>> m_words;
	QScrollArea* m_scrollArea;
	QWidget* m_scrollWidget;
	QVBoxLayout* m_layout;
};

