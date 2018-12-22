#include "ResourceCache/Resource.hpp"

#include <algorithm>
#include <cctype>

namespace Vague
{
    Resource::Resource(const std::string &_name)
    {
        m_name = _name;
        std::transform(m_name.begin(), m_name.end(), m_name.begin(), std::tolower);
    }
}
