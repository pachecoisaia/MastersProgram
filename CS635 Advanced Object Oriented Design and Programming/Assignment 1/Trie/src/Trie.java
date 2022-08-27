
import java.util.ArrayList;
import java.util.Set;

public class Trie implements ITrie {

    private TrieNode root;

    public Trie() {
        root = new TrieNode();
    }

    @Override
    public void add(String word) {
        TrieNode currentTrieNode = root; //began at the root
        for (int i = 0; i < word.length(); i++) { //traverse all the characters in word
            char character = word.charAt(i); //get a character in the word

            if (!currentTrieNode.children.containsKey(character)) { //check if the character exist in its children node
                currentTrieNode.children.put(character, new TrieNode()); //no, add the character and create new sub trie
            }
            TrieNode nextTrieNode = currentTrieNode.children.get(character); 
            currentTrieNode = nextTrieNode; //traverse to next trie
        }

        currentTrieNode.isWord = true; //tell the user its a complete word.
        currentTrieNode.word = word;
    }

    @Override
    public boolean doesWordExist(String word) {
        TrieNode wordTrie = findWord(word);
        return wordTrie != null && wordTrie.isWord;
    }

    public TrieNode findWord(String word) {

        TrieNode currentTrieNode = root; //began at root
        for (int i = 0; i < word.length(); i++) { //traverse all the characters in word
            char character = word.charAt(i); //get a character in the word

            if (!currentTrieNode.children.containsKey(character)) //check if the character exist in its children node
            {
                currentTrieNode.children.put(character, new TrieNode()); //no, add the character and create new sub trie
            }
            TrieNode nextTrieNode = currentTrieNode.children.get(character);
            currentTrieNode = nextTrieNode; //traverse to next trie
        }

        return currentTrieNode;

    }

    @Override
    public ArrayList<String> findWordsThatContain(String substring) {
        TrieNode currentTrieNode = root;
        ArrayList<String> wordsWithSubstring = new ArrayList<String>();

        if (currentTrieNode.isWord && currentTrieNode.word.contains(substring)) {
            wordsWithSubstring.add(currentTrieNode.word);
        }
        if (!currentTrieNode.children.isEmpty()) {
            Set<Character> nodes = currentTrieNode.children.keySet();

            nodes.forEach(node -> {
                TrieNode nextNode = currentTrieNode.children.get(node);
                findWordsThatContain(nextNode, wordsWithSubstring, substring);
            });
        }

        return wordsWithSubstring;
    }

    public void findWordsThatContain(TrieNode nextNode, ArrayList<String> wordsWithSubstring, String substring) {
        TrieNode currentTrieNode = nextNode;
        if (currentTrieNode.isWord && currentTrieNode.word.contains(substring)) {
            wordsWithSubstring.add(currentTrieNode.word);
        }
        if (!currentTrieNode.children.isEmpty()) {
            Set<Character> nodes = currentTrieNode.children.keySet();

            nodes.forEach(node -> {
                TrieNode nextNodeOfChildren = currentTrieNode.children.get(node);
                findWordsThatContain(nextNodeOfChildren, wordsWithSubstring, substring);
            });
        }
    }

    @Override
    public void print() {
        TrieNode currentTrieNode = root;
        if (currentTrieNode.isWord) {
            System.out.println(currentTrieNode.word);
        }
        if (!currentTrieNode.children.isEmpty()) {
            Set<Character> nodes = currentTrieNode.children.keySet();

            nodes.forEach(node
                    -> {
                TrieNode nextNode = currentTrieNode.children.get(node);
                print(nextNode);
            });
        }
    }

    public void print(TrieNode nextNode) {
        TrieNode currentTrieNode = nextNode;
        if (currentTrieNode.isWord) {
            System.out.println(currentTrieNode.word);
        }

        if (!currentTrieNode.children.isEmpty()) {
            Set<Character> nodes = currentTrieNode.children.keySet();

            nodes.forEach(node
                    -> {
                TrieNode nextNodeOfChildren = currentTrieNode.children.get(node);
                print(nextNodeOfChildren);
            });
        }
    }

    public static void main(String[] args) {
        Trie trie = new Trie();
        trie.add("isaia");
        trie.add("isaias");
        trie.add("isaiat");
        trie.doesWordExist("max");
        trie.print();
        ArrayList<String> wordsWithSubstring = trie.findWordsThatContain("a");
        System.out.println(wordsWithSubstring);

    }
}
