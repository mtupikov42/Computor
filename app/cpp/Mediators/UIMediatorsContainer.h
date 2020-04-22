#pragma once

#include <QObject>
#include <QScopedPointer>

namespace UIMediators {

class UIMediatorsContainer : public QObject {
    Q_OBJECT
	//Q_PROPERTY(QObject* directoryMediator READ getDirectoryMediator CONSTANT)

public:
	UIMediatorsContainer();
	~UIMediatorsContainer();

	//QObject* getDirectoryMediator() const;

private:
	//QScopedPointer<UIDirectoryMediator> m_directoryMediator;
};

} // end namespace UIMediators
