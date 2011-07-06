add(0, N, N).
add(s(V), N, A) :- add(V, s(N), A).
