smaller(a, b).
smaller(b, c).
smaller(c, d).
smaller(d, e).
smaller(e, f).
smaller(f, g).
smaller(g, h).
smaller(h, i).
smaller(i, j).

bigger(b, a).
bigger(c, b).
bigger(d, c).
bigger(e, d).
bigger(f, e).
bigger(g, f).
bigger(h, g).
bigger(i, h).
bigger(j, i).

lt(X, Y) :- smaller(X, Y).
lt(X, Y) :- smaller(X, Z), lt(Z, Y).

gte(X, X).
gte(X, Y) :- bigger(X, Y).
gte(X, Y) :- bigger(X, Z), gte(Z, Y).

split(X, nil, nil, nil).
split(X, cons(Y, Tail), cons(Y, Small), Big) :-
	gte(X, Y),
	split(X, Tail, Small, Big).
split(X, cons(Y, Tail), Small, cons(Y, Big)) :-
	lt(X, Y),
	split(X, Tail, Small, Big).


myAppend(nil, Y, Y).
myAppend(cons(A, X), Y, cons(A, Z)) :- myAppend(X, Y, Z).

quicksort(nil, nil).
quicksort(cons(X, Tail), Sorted) :-
	split(X, Tail, Small, Big),
	quicksort(Small, SortedSmall),
	quicksort(Big, SortedBig),
	myAppend(SortedSmall, cons(X, SortedBig), Sorted).

