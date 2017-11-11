#ifndef __TRACYEVENT_HPP__
#define __TRACYEVENT_HPP__

#include <limits>

#include "TracyVector.hpp"

namespace tracy
{

#pragma pack( 1 )

struct StringRef
{
    enum Type { Ptr, Idx };

    StringRef() : active( 0 ) {}
    StringRef( Type t, uint64_t data )
        : isidx( t == Idx )
        , active( 1 )
    {
        if( isidx )
        {
            stridx = data;
        }
        else
        {
            strptr = data;
        }
    }

    union
    {
        uint64_t strptr;
        uint64_t stridx;
    };

    uint8_t isidx   : 1;
    uint8_t active  : 1;
};

struct TextData
{
    const char* userText;
    StringRef zoneName;
};

struct SourceLocation
{
    StringRef function;
    StringRef file;
    uint32_t line;
    uint32_t color;
};

enum { SourceLocationSize = sizeof( SourceLocation ) };


struct ZoneEvent
{
    int64_t start;
    int64_t end;
    int32_t srcloc;
    int8_t cpu_start;
    int8_t cpu_end;

    int32_t text;
    Vector<ZoneEvent*> child;
};

enum { ZoneEventSize = sizeof( ZoneEvent ) };


struct LockEvent
{
    enum class Type : uint8_t
    {
        Wait,
        Obtain,
        Release
    };

    int64_t time;
    int32_t srcloc;
    uint64_t waitList;
    uint16_t thread         : 6;
    uint16_t lockingThread  : 6;
    uint16_t type           : 2;
    uint8_t lockCount;
};

enum { LockEventSize = sizeof( LockEvent ) };

enum { MaxLockThreads = sizeof( LockEvent::waitList ) * 8 };
static_assert( std::numeric_limits<decltype(LockEvent::lockCount)>::max() >= MaxLockThreads, "Not enough space for lock count." );

#pragma pack()


struct MessageData
{
    int64_t time;
    StringRef ref;
};

struct ThreadData
{
    uint64_t id;
    bool showFull;
    bool visible;
    Vector<ZoneEvent*> timeline;
    Vector<MessageData*> messages;
};

struct LockMap
{
    uint32_t srcloc;
    Vector<LockEvent*> timeline;
    std::unordered_map<uint64_t, uint8_t> threadMap;
    std::vector<uint64_t> threadList;
    bool visible;
};

struct LockHighlight
{
    int64_t id;
    int64_t begin;
    int64_t end;
    uint8_t thread;
    bool blocked;
};

struct PlotItem
{
    int64_t time;
    double val;
};

struct PlotData
{
    uint64_t name;
    double min;
    double max;
    bool showFull;
    bool visible;
    Vector<PlotItem*> data;
    Vector<PlotItem*> postpone;
    uint64_t postponeTime;
};

struct StringLocation
{
    const char* ptr;
    uint32_t idx;
};

struct SourceLocationHasher
{
    size_t operator()( const SourceLocation* ptr ) const
    {
        return charutil::hash( (const char*)ptr, sizeof( SourceLocation ) );
    }
};

struct SourceLocationComparator
{
    bool operator()( const SourceLocation* lhs, const SourceLocation* rhs ) const
    {
        return memcmp( lhs, rhs, sizeof( SourceLocation ) ) == 0;
    }
};

}

#endif
