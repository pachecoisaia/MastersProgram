
import java.util.ArrayList;

public interface ITrie {

    public void add(String word);

    public boolean doesWordExist(String word);

    public TrieNode findWord(String word);

    public ArrayList<String> findWordsThatContain(String substring);

    public void print();
}
