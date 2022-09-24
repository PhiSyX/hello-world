import Data.List (permutations)
import qualified Data.Map.Strict as Map

type Distance    =         (String, String, Int)
type DistanceMap = Map.Map (String, String) Int

include_bytes :: IO String
include_bytes = readFile "../puzzles/day09.txt"

find_distances :: [Distance] -> DistanceMap
find_distances = Map.fromList . concatMap pairs
  where pairs ( a, b, d ) = [ ( ( a, b ), d ), ( ( b, a ), d ) ]

total_distance :: DistanceMap -> [String] -> Int
total_distance m path = sum ( map ( m Map.! ) pairs )
  where pairs = zip path (drop 1 path)

places :: [Distance] -> [String]
places [] = []
places line @ ( ( place1, _, _ ) : _ ) = place1 : args
    where args = map snd3 ( takeWhile ( ( place1 == ) . fst3 ) line )

parse :: [String] -> Distance
parse [ a, "to", b, "=", distance ] = ( a, b, read distance )
parse w = error ("impossible d'analyser la distance: " ++ unwords w)

fst3 :: (a, b, c) -> a
fst3 (x, _, _) = x

snd3 :: (a, b, c) -> b
snd3 (_, x, _) = x

-- Main

main :: IO ()
main = do
    putStrLn $ "--- Day 9: All in a Single Night ---"
    input <- include_bytes
    let line = map ( parse . words ) ( lines input )

    let distances = find_distances line
    let paths     = permutations ( places line )
    let totals    = map ( total_distance distances ) paths

    putStrLn $ "--- Part One ---"
    putStrLn $ "\t Your puzzle answer is " ++ show ( minimum totals )
    putStrLn $ "--- Part Two ---"
    putStrLn $ "\t Your puzzle answer is " ++ show ( maximum totals )
