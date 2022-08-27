
import java.util.HashMap;

public class TrieNode {

    public String word;
    public boolean isWord = false;
    public HashMap<Character, TrieNode> children;

    public TrieNode() {
        word = "";
        isWord= false;
        children = new HashMap<>();
    }

    @Override
    public String toString() {
        return "[word: " + word + ", children: " + children.toString() + "]";
    }
}
