
import java.util.HashMap;

public class TrieNode {
    public Character character;
    public String word;
    public boolean isWord = false;
    public HashMap<Character, TrieNode> children;

    public TrieNode() {
        character = null;
        word = null;
        isWord = false;
        children = new HashMap<>();
    }
    
    public TrieNode(Character character) {
        this.character = character;
        word = null;
        isWord = false;
        children = new HashMap<>();
    }

    @Override
    public String toString() {
        return "[character: " + character + ", word: " + word + ", isWord: " + isWord + ", children: " + children.toString() + "]";
    }
}
