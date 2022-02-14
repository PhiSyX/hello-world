program main
   implicit none

   character :: ch
   character(len=5) :: str

   ch = 'h'
   str = "hello world"

   print '(A, I0)', 'len(ch) = ', len(ch)
   print '(A, I0)', 'len(str) = ', len(str)

   str = "abc"
   print '(A, I0)', 'len(str) = ', len(str)
   print '(A, I0)', 'len_trim(str) = ', len_trim(str)
end program main
