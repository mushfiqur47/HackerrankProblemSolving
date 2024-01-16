import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.TreeSet;

public class Solution {
    static TreeSet<String>t;
    public static void main(String[] args) {
        try{
        BufferedReader br=new BufferedReader(new InputStreamReader(System.in));
        t=new TreeSet<String>();
        int n=Integer.parseInt(br.readLine());
        for(int i=0;i<n;i++){
        String s=br.readLine();
        for(int j=0;j<s.length();j++){
        t.add(s.substring(j,s.length()));
        }
        }
        Object [] suffix1=(t.toArray());
        String suffix[]=new String[suffix1.length];
        for(int l=0;l<suffix.length;l++){
        suffix[l]=(String)suffix1[l];
        //System.out.println(suffix[l]);
        }
        int len[]=new int[suffix.length];
        int lcp[]=new int[suffix.length];
        len[0]=suffix[0].toString().length();
        lcp[0]=0;
        for(int j=1;j<suffix.length;j++){
        int count=0;
        try{
        while(true){
        if(suffix[j-1].charAt(count)==suffix[j].charAt(count)){
        count++;
        }
        else{
        break;
        }        
        }}catch(StringIndexOutOfBoundsException e){}
        len[j]=suffix[j].length()-count;
        lcp[j]=count;
        }
       int q=Integer.parseInt(br.readLine());
       for(int i=0;i<q;i++){
       int a=Integer.parseInt(br.readLine());
       int a1=0;
       int j=0;
       int count=0;
       try{
       while(j<a){
       a1=j;
       j=j+len[count++];
       }
       count--;
       System.out.println(suffix[count].substring(0, lcp[count]+a-a1));
       }catch(ArrayIndexOutOfBoundsException e){
       System.out.println("INVALID");
       }
       }         
        }catch(IOException e){
        System.out.println(e);        
        }

    }

}
