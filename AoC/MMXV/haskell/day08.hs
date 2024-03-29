include_bytes :: IO String
include_bytes = readFile "../puzzles/day08.txt"

solve_part01 :: String -> Int
solve_part01 ('"': line) = 1 + overhead line
    where   overhead ( '\\': '\\' : ch         )   = 1 + overhead ch
            overhead ( '\\': '"'  : ch         )   = 1 + overhead ch
            overhead ( '\\': 'x'  : _ : _ : ch )   = 3 + overhead ch
            overhead [ '"' ]                       = 1
            overhead ( _ : ch )                    = overhead ch
            overhead []                            = error ( "ne termine pas par une quote: " ++ line )
solve_part01 line = error ( "ne commence pas par une quote: " ++ line )

solve_part02 :: String -> Int
solve_part02 s = 2 + count '"' s + count '\\' s

sum_by :: (n -> Int) -> [n] -> Int
sum_by f = sum . map f

count :: Char -> String -> Int
count ch = length . filter (== ch)

main :: IO ()
main = do
    putStrLn $ "--- Day 8: Matchsticks ---"
    input <- include_bytes
    let line = lines input

    putStrLn $ "--- Part One ---"
    putStrLn $ "\t Your puzzle answer is " ++ show ( sum_by solve_part01 line )
    putStrLn $ "--- Part Two ---"
    putStrLn $ "\t Your puzzle answer is " ++ show ( sum_by solve_part02 line )
