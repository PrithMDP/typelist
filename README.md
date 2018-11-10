# TypeList

This is an attempt to implement a comile time `typelist` which essentially a set that contains different types at compile time. 

Inspiration: http://www.drdobbs.com/generic-programmingtypelists-and-applica/184403813

## Operations

Currently `intersection` and `union` are supported by using the templated structs `intersection` and `combiner` respectively.

Would like to add `addition`, `subtraction` and other set operations as well. 

## How it works

The basic structure is this:

```
template <typename... Args>
struct Tlist
{
};
```
`Tlist` can hold an arbritary number and type of `types`. This forms the basis which holds the concrete types.

Lets now take a look at the `intersection` implementation. This is a bit simpler than `combiner` so its better to start with this. 

The following declaration defines a `struct` that takes in 3 templated arguments
```
template <typename A, typename B, typename C>
struct intersection
{
};
```
To perform intersection, I recursively iterate over the "head" element in the first typelist and "peel off" the types from the first one by one. When performing the "Peel" I check if the type is present in the second typelist. If it is, then I check if it has been already added to the output typelist (this happens when we have duplicate types, which is perfectly legal) if it is not already present, I add it to the output typelist.


If both lists are empty, there is nothing to be done! 
```
// Base, when both are empty
template <typename... R1>
struct intersection<Tlist<>, Tlist<>, Tlist<R1...>>
{
    using val = Tlist<R1...>;
};
```

Here we specialize on the case where the first list is empty. When this happens we are done! 
```
//if first list is empty we are done!
template <typename A, typename... L1, typename... R1>
struct intersection<Tlist<>, Tlist<A,L1...>, Tlist<R1...>>
{
    using val = Tlist<R1...>;
};

```

This is the most interesting portion: I peel off the types one by one, check if it is in the other list and then if its not in the output list, I add to the output list the current type `A`.

I use the helper struct `exists` to loop over the `variadic params`.

When the first list becomes empty, the recursive call `intersection<Tlist<L1...>,Tlist<L2...>,lst>::val` hits the empty first list  specialization and we are done! 
```
template <typename A, typename... L1, typename... L2, typename... R1>
struct intersection< Tlist<A,L1...>, Tlist<L2...>,Tlist<R1...> >
{
    static const bool AExistsinB = exists<A,L2...>::val;
    static const bool AExistsinR = exists<A,R1...>::val;
    
    // if in L2 and not in R1 add
    // if in L2 and in R1 do not add
    using lst = typename std::conditional<AExistsinB, typename std::conditional<AExistsinR, Tlist<R1...>,Tlist<A,R1...>>::type,Tlist<R1...>>::type;
    
    using val = typename intersection<Tlist<L1...>,Tlist<L2...>,lst>::val;
};
```
