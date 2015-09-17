#include "Connection.h"

Connection::Connection(int f, int t, ConnectionTypes tp)
    :from(f), to(t), types(tp)
{
} 

bool Connection::operator<(const Connection& other) const
{
    return from < other.from;
}

bool Connection::operator==(const Connection& other) const
{
    return from == other.from &&
            to == other.from &&
            types == other.types;
}
