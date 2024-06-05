/**
 * @file StateMachineLib.h
 * @brief Declaración de la clase StateMachine para implementar una máquina de estados.
 */

#ifndef _StateMachineLib_h
#define _StateMachineLib_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

/**
 * @class StateMachine
 * @brief Implementa una máquina de estados con transiciones y acciones asociadas.
 */
class StateMachine
{
    /**
     * @brief Tipo de condición para la transición de estados.
     * 
     * @return true si la condición se cumple.
     * @return false si la condición no se cumple.
     */
    typedef bool(*StateMachineCondition)();

    /**
     * @brief Tipo de acción a ejecutar en la máquina de estados.
     */
    typedef void(*StateMachineAction)();

    /**
     * @struct State
     * @brief Estructura que representa un estado con acciones asociadas.
     */
    typedef struct {
        StateMachineAction OnEntering; ///< Acción al entrar en el estado.
        StateMachineAction OnLeaving; ///< Acción al salir del estado.
    } State;

    /**
     * @struct Transition
     * @brief Estructura que representa una transición entre dos estados.
     */
    typedef struct {
        uint8_t InputState; ///< Estado de entrada de la transición.
        uint8_t OutputState; ///< Estado de salida de la transición.

        StateMachineCondition Condition; ///< Condición para realizar la transición.
        StateMachineAction Action; ///< Acción a ejecutar durante la transición.
    } Transition;

public:
    /**
     * @brief Constructor de la clase StateMachine.
     * 
     * @param numStates Número de estados en la máquina de estados.
     * @param numTransitions Número de transiciones en la máquina de estados.
     */
    StateMachine(uint8_t numStates, uint8_t numTransitions);

    /**
     * @brief Añade una transición entre estados sin acción asociada.
     * 
     * @param inputState Estado de entrada de la transición.
     * @param outputState Estado de salida de la transición.
     * @param condition Condición que debe cumplirse para realizar la transición.
     * @return Índice de la transición añadida.
     */
    uint8_t AddTransition(uint8_t inputState, uint8_t outputState, StateMachineCondition condition);

    /**
     * @brief Añade una transición entre estados con una acción asociada.
     * 
     * @param inputState Estado de entrada de la transición.
     * @param outputState Estado de salida de la transición.
     * @param condition Condición que debe cumplirse para realizar la transición.
     * @param action Acción a ejecutar durante la transición.
     * @return Índice de la transición añadida.
     */
    uint8_t AddTransition(uint8_t inputState, uint8_t outputState, StateMachineCondition condition, StateMachineAction action);

    /**
     * @brief Configura una transición sin acción asociada.
     * 
     * @param transition Índice de la transición.
     * @param inputState Estado de entrada de la transición.
     * @param outputState Estado de salida de la transición.
     * @param condition Condición que debe cumplirse para realizar la transición.
     */
    void SetTransition(uint8_t transition, uint8_t inputState, uint8_t outputState, StateMachineCondition condition);

    /**
     * @brief Configura una transición con una acción asociada.
     * 
     * @param transition Índice de la transición.
     * @param inputState Estado de entrada de la transición.
     * @param outputState Estado de salida de la transición.
     * @param condition Condición que debe cumplirse para realizar la transición.
     * @param action Acción a ejecutar durante la transición.
     */
    void SetTransition(uint8_t transition, uint8_t inputState, uint8_t outputState, StateMachineCondition condition, StateMachineAction action);

    /**
     * @brief Elimina la condición de una transición.
     * 
     * @param transition Índice de la transición.
     */
    void RemoveTransition(uint8_t transition);

    /**
     * @brief Configura una acción al entrar en un estado.
     * 
     * @param state Índice del estado.
     * @param action Acción a ejecutar al entrar en el estado.
     */
    void SetOnEntering(uint8_t state, StateMachineAction action);

    /**
     * @brief Configura una acción al salir de un estado.
     * 
     * @param state Índice del estado.
     * @param action Acción a ejecutar al salir del estado.
     */
    void SetOnLeaving(uint8_t state, StateMachineAction action);

    /**
     * @brief Elimina la acción al entrar en un estado.
     * 
     * @param state Índice del estado.
     */
    void ClearOnEntering(uint8_t state);

    /**
     * @brief Elimina la acción al salir de un estado.
     * 
     * @param state Índice del estado.
     */
    void ClearOnLeaving(uint8_t state);

    /**
     * @brief Cambia el estado actual de la máquina de estados.
     * 
     * @param state Índice del nuevo estado.
     * @param launchLeaving Indica si se debe ejecutar la acción al salir del estado actual.
     * @param launchEntering Indica si se debe ejecutar la acción al entrar en el nuevo estado.
     */
    void SetState(uint8_t state, bool launchLeaving, bool launchEntering);

    /**
     * @brief Obtiene el estado actual de la máquina de estados.
     * 
     * @return Índice del estado actual.
     */
    uint8_t GetState() const;

    /**
     * @brief Actualiza la máquina de estados, verificando las condiciones de transición.
     * 
     * @return true si se realizó una transición, false en caso contrario.
     */
    bool Update();

private:
    State* _states; ///< Array de estados.
    uint8_t _numStates; ///< Número de estados.
    uint8_t _currentStateIndex; ///< Índice del estado actual.

    Transition* _transitions; ///< Array de transiciones.
    uint8_t _numTransitions; ///< Número de transiciones.
    uint8_t _currentTransitionIndex; ///< Índice de la transición actual.

    /**
     * @brief Inicializa las variables de la máquina de estados.
     */
    void initVariables();
};

#endif
