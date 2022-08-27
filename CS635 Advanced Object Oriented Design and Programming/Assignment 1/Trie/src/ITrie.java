
public interface ITrie {

    public void add(String word);

    public boolean find(String word);

    public boolean startsWith(String prefix);

    public void print();

    public void findAllWordsWithSubString(String substring);
}
