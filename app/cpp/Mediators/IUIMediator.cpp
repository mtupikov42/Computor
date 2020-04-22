#include "Mediators/IUIMediator.h"
#include "Mediators/UIMediatorsContainer.h"

namespace UIMediators {

IUIMediator::IUIMediator(UIMediatorsContainer* _mediators_container) :
    mediators_container(_mediators_container)
{
}

UIMediatorsContainer* IUIMediator::getMediatorsContainer() const {
    return mediators_container;
}

} // end namespace UIMediators
