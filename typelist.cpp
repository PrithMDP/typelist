#include <iostream>
#include <vector>
#include <string>
#include <type_traits>

using namespace std; // forgive me 

template <typename T>
struct WhichType;

template <typename... Args>
struct Tlist
{
    template <typename AddType>
    Tlist<Args...,AddType> append_type()
    {
        return {};
    }
};


// helper to check if var arg contains a type
template <typename In, typename... Rest>
struct exists{
    static const bool val = false;
};

template <typename In, typename Last>
struct exists<In, Last>
{
    static const bool val = is_same_v<In,Last>;
};
template <typename In, typename First, typename... Rest>
struct exists<In,First,Rest...>
{
    static const bool val = is_same_v<In,First> ? true : exists<In,Rest...>::val;
};


//base template
template <typename A, typename B, typename C>
struct combiner
{
    
};

// Base, when both are empty
template <typename... R1>
struct combiner<Tlist<>, Tlist<>, Tlist<R1...>>
{
    using val = Tlist<R1...>;
};

// when first list is empty
template <typename A, typename... L1, typename... R1>
struct combiner<Tlist<>, Tlist<A,L1...>, Tlist<R1...>>
{
    static const bool AExists = exists<A,R1...>::val;
    
    using lst = typename std::conditional<AExists, Tlist<R1...>, Tlist<A,R1...> >::type;
    
    using val = typename combiner<Tlist<>,Tlist<L1...>,lst>::val;   
};


// when second list is empty
template <typename A, typename... L1, typename... R1>
struct combiner<Tlist<A,L1...>, Tlist<>, Tlist<R1...>>
{
    static const bool AExists = exists<A,R1...>::val;
    
    using lst = typename std::conditional<AExists, Tlist<R1...>, Tlist<A,R1...> >::type;
    
    using val = typename combiner<Tlist<L1...>,Tlist<>,lst>::val;   
};



// input, ouput, and result type;
template <typename A, typename B, typename... L1, typename... L2, typename... R1>
struct combiner< Tlist<A,L1...>, Tlist<B,L2...>,Tlist<R1...> >
{
    // check if they are same
    // and check if already in R1
    static const bool AExists = exists<A,R1...>::val;
    static const bool BExists = exists<B,R1...>::val;
    
    using lst = typename std::conditional<AExists, typename std::conditional<BExists,Tlist<R1...>,Tlist<B,R1...>>::type, typename std::conditional<BExists,Tlist<A,R1...>,Tlist<A,B,R1...>>::type>::type;
    
    using val = typename combiner<Tlist<L1...>,Tlist<L2...>,lst>::val;   
};



template <typename A, typename B, typename C>
struct intersection
{
    
};

// Base, when both are empty
template <typename... R1>
struct intersection<Tlist<>, Tlist<>, Tlist<R1...>>
{
    using val = Tlist<R1...>;
};
//if either list is empty we are done!
template <typename A, typename... L1, typename... R1>
struct intersection<Tlist<>, Tlist<A,L1...>, Tlist<R1...>>
{
    using val = Tlist<R1...>;
};

template <typename A, typename... L1, typename... R1>
struct intersection<Tlist<A,L1...>, Tlist<>, Tlist<R1...>>
{
    using val = Tlist<R1...>;
};

template <typename A, typename B, typename... L1, typename... L2, typename... R1>
struct intersection< Tlist<A,L1...>, Tlist<B,L2...>,Tlist<R1...> >
{
    static const bool AExistsinB = exists<A,L2...>::val;
    static const bool AExistsinR = exists<A,R1...>::val;
    
    // if in L2 and not in R1 add
    // if in L2 and in R1 do not add
    using lst = typename std::conditional<AExistsinB, typename std::conditional<AExistsinR, Tlist<R1...>,Tlist<A,R1...>>::type,Tlist<R1...>>::type;
    
    using val = typename intersection<Tlist<L1...>,Tlist<L2...>,lst>::val;
};

int main() {
    
    // using this type struct to check which types are infact contained in the alias 'val'
    // comment either to make the other fail :) 
    WhichType<intersection<Tlist<short,float>,Tlist<int,double,short,float>,Tlist<>>::val> g;
    WhichType<combiner<Tlist<short>,Tlist<int,double,char,float>,Tlist<>>::val> g;
    
}


