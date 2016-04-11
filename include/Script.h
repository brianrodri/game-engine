#pragma once
#include <unordered_map>


/**
 * A stateful container of strings that increments and changes subsequently
 * returned strings based upon optional input.
 *
 * Should be inherited from to allow more advanced forms of state-changes.  This
 * base-class only allows optional integer inputs which when at a state w/ an
 * optional set of choices will use it to choose an option out of a simple list.
 * Again, derive from this class to create more interesting Scripts.
 */

struct SimpleScript {

    SimpleScript();


    //! Creates a replica of `this` script.
    /**
     *  Allows a base-script to exist and the transcription of more for
     *  convenience.
     */
    SimpleScript(const SimpleScript&);

    /**
     * @brief Advances to next state
     *
     * @param input
     *
     * @return 
     */
    std::string advance(int input = 0);

    //! Returns the current text the script-state is resting at
    const std::string& currentText() const;

protected:

    std::unordered_map<std::string, std::string>  m_conversations;

} /*struct Script*/;
