// C++14/C++17 finite state machine example based on code from Mateusz Pusz,
// https://github.com/mpusz/fsm-variant (MIT-license)

// Note: unlike std::visit(), variant-lite's visit() passes the variant by const &.

#include "nonstd/optional.hpp"
#include "nonstd/variant.hpp"

// Generic state machine state-event dispatch:

template< typename Derived, typename StateVariant >
class fsm
{
public:
    const StateVariant & state() const
    {
        return m_state;
    }

    StateVariant & state()
    {
        return m_state;
    }

    template<typename Event>
    void dispatch( Event && event )
    {
        Derived & child = static_cast<Derived &>( *this );

        if ( auto new_state = nonstd::visit(
            [&]( auto & s ) -> nonstd::optional<StateVariant> {
                return { child.on_event( s, std::forward<Event>( event ) ) };
            }, m_state )
        )
        {
            m_state = *std::move( new_state );
        }
    }

private:
    StateVariant m_state;
};

// Specific state machine:

#include <iostream>

struct event_start{};
struct event_stop{};
struct event_pause{};

struct state_idle{};
struct state_running{};
struct state_paused{};

using state = nonstd::variant<state_idle, state_running, state_paused>;

class sample_fsm : public fsm<sample_fsm, state>
{
public:
    template<typename State, typename Event>
    auto on_event( State const &, Event const & )
    {
        std::cout << "[unsupported transition]\n";
        return nonstd::nullopt;
    }

    auto on_event( state_idle const &, event_start const & )
    {
        std::cout << "idle|start -> running\n";
        return state_running{};
    }

    auto on_event( state_running const &, event_pause const & )
    {
        std::cout << "running|pause -> paused\n";
        return state_paused{};
    }

    auto on_event( state_running const &, event_stop const & )
    {
        std::cout << "running|stop -> idle\n";
        return state_idle{};
    }

    auto on_event( state_paused const &, event_start const & )
    {
        std::cout << "paused|start -> running\n";
        return state_running{};
    }
    auto on_event( state_paused const &, event_stop const & )
    {
        std::cout << "paused|stop -> idle\n";
        return state_idle{};
    }
};

int main()
{
    sample_fsm fsm;
    fsm.dispatch( event_start() );
    fsm.dispatch( event_pause() );
    fsm.dispatch( event_start() );
    fsm.dispatch( event_stop() );
}

// At default:
// - with C++17, std::optional. std::variant is used (via namespace nonstd),
// - with C++14, nonstd::optional and nonstd::variant are used.

// To explicitly select std, nonstd optional, variant, add:
// -D{xxx}_CONFIG_SELECT_{XXX}={xxx}_{XXX}_DEFAULT
// -D{xxx}_CONFIG_SELECT_{XXX}={xxx}_{XXX}_NONSTD
// -D{xxx}_CONFIG_SELECT_{XXX}={xxx}_{XXX}_STD
// where:
// {xxx}: optional, variant
// {XXX}: OPTIONAL, VARIANT

// cl -std:c++17 -I../include -I../../optional-lite/include -EHsc 04-state-machine.cpp && 04-state-machine.exe
// cl -std:c++14 -I../include -I../../optional-lite/include -EHsc 04-state-machine.cpp && 04-state-machine.exe

// g++ -std=c++17 -I../include -I../../optional-lite/include -o 04-state-machine.exe 04-state-machine.cpp && 04-state-machine.exe
// g++ -std=c++14 -I../include -I../../optional-lite/include -o 04-state-machine.exe 04-state-machine.cpp && 04-state-machine.exe
