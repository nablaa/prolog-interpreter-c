ackermann(s(s(0)), s(s(0)), Result).
% Yes.
% Solution: ackermann(s(s(0)),s(s(0)),s(s(s(s(s(s(s(0)))))))).


ackermann(s(s(0)), Value, s(s(s(s(s(s(s(0)))))))).
% Yes.
% Solution: ackermann(s(s(0)),s(s(0)),s(s(s(s(s(s(s(0)))))))).


ackermann(Value1, Value2, s(s(s(s(s(0)))))).
% Yes.
% Solution: ackermann(0,s(s(s(s(0)))),s(s(s(s(s(0)))))).
% More solutions (y/n) ?
% y
% Yes.
% Solution: ackermann(s(s(s(0))),0,s(s(s(s(s(0)))))).

quit.
