#pragma once

#include <QObject>

namespace UIMediators {

class UIMediatorsContainer;

class IUIMediator : public QObject {
    Q_OBJECT

public:
	IUIMediator(UIMediatorsContainer* mediators_container);

protected:
	UIMediatorsContainer* getMediatorsContainer() const;

private:
	UIMediatorsContainer* mediators_container;
};

} // end namespace UIMediators
