
import java.util.ArrayList;
import java.util.Set;

public class Trie implements ITrie {

    private TrieNode root;

    public Trie() {
        root = new TrieNode();
    }

    @Override
    public void add(String word) {
        TrieNode currentTrieNode = root;//began at root
        for (int i = 0; i < word.length(); i++) {
            char character = word.charAt(i);//get character in word

            if (!currentTrieNode.children.containsKey(character)) { //does the characters exist in the trie?
                currentTrieNode.children.put(character, new TrieNode()); //no, create a new sub trie
            }
            currentTrieNode = currentTrieNode.children.get(character); //traverse to new trie
        }

        currentTrieNode.isWord = true; //tell the user its a complete word.
        currentTrieNode.word = word;
    }

    @Override
    public boolean find(String word) {
        TrieNode subTrie = getTrieNode(word);
        return subTrie != null && subTrie.isWord;
    }

    @Override
    public void print() {
        TrieNode currentTrieNode = root;
        if (currentTrieNode.isWord) {
            System.out.println(currentTrieNode.word);
        }
        if (!currentTrieNode.children.isEmpty()) {
            Set<Character> keys = currentTrieNode.children.keySet();

            keys.forEach(key
                    -> {
                print(currentTrieNode.children.get(key));
            });
        }
    }

    @Override
    public ArrayList<String> findWordsThatContain(String substring) {
        TrieNode currentTrieNode = root;
        ArrayList<String> validWords = new ArrayList<String>();

        if (currentTrieNode.isWord && currentTrieNode.word.contains(substring)) {
            validWords.add(currentTrieNode.word);
        }
        if (!currentTrieNode.children.isEmpty()) {
            Set<Character> keys = currentTrieNode.children.keySet();

            keys.forEach(key
                    -> {
                findWordsThatContain(currentTrieNode.children.get(key), validWords, substring);
            });
        }

        return validWords;
    }

    public void print(TrieNode node) {
        TrieNode currentTrieNode = node;
        if (currentTrieNode.isWord) {
            System.out.println(currentTrieNode.word);
        }

        if (!currentTrieNode.children.isEmpty()) {
            Set<Character> keys = currentTrieNode.children.keySet();

            keys.forEach(key
                    -> {
                print(currentTrieNode.children.get(key));
            });
        }
    }

    public TrieNode getTrieNode(String word) {

        TrieNode currentTrieNode = root;//began at root
        for (int i = 0; i < word.length(); i++) {
            char character = word.charAt(i);//get character in word

            if (!currentTrieNode.children.containsKey(character)) //does the characters exist in the trie?
            {
                currentTrieNode.children.put(character, new TrieNode()); //no, create a new sub trie
            }
            currentTrieNode = currentTrieNode.children.get(character); //traverse to new trie
        }

        return currentTrieNode;

    }

    public void findWordsThatContain(TrieNode node, ArrayList<String> validWords, String substring) {
        TrieNode currentTrieNode = node;
        if (currentTrieNode.isWord && currentTrieNode.word.contains(substring)) {
            validWords.add(currentTrieNode.word);
        }
        if (!currentTrieNode.children.isEmpty()) {
            Set<Character> keys = currentTrieNode.children.keySet();

            keys.forEach(key
                    -> {
                findWordsThatContain(currentTrieNode.children.get(key), validWords, substring);
            });
        }
    }

    public static void main(String[] args) {
        Trie trie = new Trie();
        trie.add("isaia");
        trie.add("isaias");
        trie.add("isaiat");
        trie.find("max");
        trie.print();
        
        ArrayList<String> wordsWithSubstring = trie.findWordsThatContain("a");
        System.out.println(wordsWithSubstring);

    }
}
