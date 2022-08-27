
import java.util.ArrayList;


public interface ITrie {

    public void add(String word);

    public boolean find(String word);

    public void print();

    public ArrayList<String> findWordsThatContain(String substring);
}
