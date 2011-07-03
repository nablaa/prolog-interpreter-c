% Base, A(0, N) = N + 1
ackermann(0,N,s(N)).
% Simple recursion A(M+1, 0) = A(M, 1)
ackermann(s(M), 0, Val) :-
	ackermann(M, s(0), Val).
% Complicated recursion A(M+1, N+1) = A(M, A(M, N))
ackermann(s(M), s(N), Val) :-
	ackermann(s(M), N, Val1),
        ackermann(M, Val1, Val).

% Sample queries
% Query: ackermann(s(s(0)), s(s(0)), Result).
% Yes.
% Solution: ackermann(s(s(0)),s(s(0)),s(s(s(s(s(s(s(0)))))))).
%
% Query: ackermann(s(s(0)), Value, s(s(s(s(s(s(s(0)))))))).
% Yes.
% Solution: ackermann(s(s(0)),s(s(0)),s(s(s(s(s(s(s(0)))))))).
%
% Query: ackermann(Value1, Value2, s(s(s(s(s(0)))))).
% Yes.
% Solution: ackermann(0,s(s(s(s(0)))),s(s(s(s(s(0)))))).
% More solutions (y/n) ?
% y
% Yes.
% Solution: ackermann(s(s(s(0))),0,s(s(s(s(s(0)))))).
