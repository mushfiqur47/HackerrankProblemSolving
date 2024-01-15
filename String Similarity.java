


import java.io.*;
import java.util.*;

public class Solution {

    public static void main(String[] args) {
        /* Enter your code here. Read input from STDIN. Print output to STDOUT. Your class should be named Solution. */
        Scanner s = new Scanner(System.in);
        int q = s.nextInt();
        for(int i = 0; i < q; i++){
            String st = s.next();
            long total = zFunc(st);
            System.out.println(total + st.length());
        }
    }
    public static long zFunc(String st){
        int n = st.length();
        char[] s = st.toCharArray();
        long total = 0;
        long[] z = new long[n];
        int L = 0, R = 0;
        for (int i = 0; i < n; i++) {
          if (i > R) {
            L = R = i;
            while (R < n && s[R-L] == s[R]) R++;
            z[i] = R-L; R--;
          } else {
            int k = i-L;
            if (z[k] < R-i+1) z[i] = z[k];
            else {
              L = i;
              while (R < n && s[R-L] == s[R]) R++;
              z[i] = R-L; R--;
            }
          }
          total+=z[i];
        }
        return total;
    }
}
