#include "GameVisitors.h"

UpdateVisitor::UpdateVisitor(float dtIn) : m_dt{dtIn}
{
}

DrawVisitor::DrawVisitor(sf::RenderTarget& targetIn, sf::RenderStates states) : m_target{targetIn}, m_states{states}
{
}
