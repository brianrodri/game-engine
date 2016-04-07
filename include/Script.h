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
struct Script {

    //! An empty script 
    Script();

    //! I encourage derived classes!  >:)
    virtual ~Script();

    //! Creates a replica of the script.
    /**
     * Allows a base-script to exist and the transcription of more for
     * convenience.
     *
     * @param   src     The script to base this Script's state upon
     */
    Script(const Script&);

    //! Advances to the next script.
    /**
     * When the current state points to an optional set of choices, the input
     * value will be used to decide the next state.
     */
    std::string advance(int input = 0);

    //! Returns the current text the script-state is resting at
    const std::string& currentText() const;

protected:

    std::unordered_map<std::string, std::string>  m_conversations;

} /*struct Script*/;
