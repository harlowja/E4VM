-module(test1).
-compile([export_all]).

size({Size, _}) when is_integer(Size), Size >= 0 ->
    X = 0,
    Size + X.
