include_bytes :: IO String
include_bytes = readFile "../puzzles/day10.txt"

look_and_say :: String -> String
look_and_say = lookup_until ""

lookup_until [] ( n : r ) = lookup_until [n] r
lookup_until buffer @ ( last : _ ) ( n : r ) | last == n = lookup_until ( n : buffer ) r
lookup_until buffer @ ( last : _ ) ( n : r ) | last /= n = (say buffer) ++ lookup_until [n] r
lookup_until buffer [] = say buffer

say :: String -> String
say p = (show $ length p) ++ [(head p)]

-- n times
r' :: Int -> (a -> a) -> a -> a
r' 0 _ p = p
r' n f p = f $ r' (n - 1) f p
t' :: Int -> String -> String
t' n input = r' n look_and_say input

-- Main

main :: IO ()
main = do
    input <- include_bytes

    let s = input

    putStrLn $ "--- Day 10: Elves Look, Elves Say ---"

    putStrLn $ "--- Part One ---"
    let part01 = t' 40 s
    putStrLn $ "\t Your puzzle answer is " ++ show (length part01)

    putStrLn $ "--- Part Two ---"
    let part02 = t' 10 part01
    putStrLn $ "\t Your puzzle answer is " ++ show (length part02)
