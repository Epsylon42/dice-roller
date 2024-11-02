#pragma once

#include "LittleFS.h"

template< typename T >
using IdContainer = T;

template< typename T >
struct DiffContainer {
private:
    T& m_ref;
    bool m_changed = true;

public:
    DiffContainer(T& ref) : m_ref{ref} {}

    bool changed() const {
        return m_changed;
    }

    void reset() {
        m_changed = false;
    }

    T get() const {
        return m_ref;
    }

    void set(T value) {
        if (value != m_ref) {
            m_ref = value;
            m_changed = true;
        }
    }
};

template< typename T >
struct DiffContainerOwning : DiffContainer<T> {
    T storage;

    DiffContainerOwning(T initial_value = T{})
        : storage{initial_value}
        , DiffContainer<T>{storage}
    {
    }
};
