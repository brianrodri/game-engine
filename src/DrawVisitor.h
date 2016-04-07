#include "DrawVisitor.h"

DrawVisitor::DrawVisitor(sf::RenderTarget& targetIn, sf::RenderStates states) : m_target{targetIn}, m_states{states}
{
}
