
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

    @Override
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
        TrieNode currentTrieNode = root; //began at the root
        ArrayList<String> wordsWithSubstring = new ArrayList<String>(); //storage for the words with the substring 

        if (currentTrieNode.isWord && currentTrieNode.word.contains(substring)) { //check if the node is a word and contains the substring
            wordsWithSubstring.add(currentTrieNode.word); //yes, add the word to the storage
        }
        if (!currentTrieNode.children.isEmpty()) { //check if the node has children
            Set<Character> nodes = currentTrieNode.children.keySet();

            nodes.forEach(node -> {
                TrieNode nextNode = currentTrieNode.children.get(node); //get all the children nodes
                findWordsThatContain(nextNode, wordsWithSubstring, substring); //repeat for children nodes
            });
        }

        return wordsWithSubstring;
    }

    public void findWordsThatContain(TrieNode nextNode, ArrayList<String> wordsWithSubstring, String substring) {
        TrieNode currentTrieNode = nextNode; //began at the childrens node
        if (currentTrieNode.isWord && currentTrieNode.word.contains(substring)) { //check if the children node is a word and contains the substring
            wordsWithSubstring.add(currentTrieNode.word); //yes, add the word to the storage
        }
        if (!currentTrieNode.children.isEmpty()) { //check if the children node has children
            Set<Character> nodes = currentTrieNode.children.keySet();

            nodes.forEach(node -> {
                TrieNode nextNodeOfChildren = currentTrieNode.children.get(node); //get all the children nodes of the children node
                findWordsThatContain(nextNodeOfChildren, wordsWithSubstring, substring); //repeat for children nodes of the children nodes
            });
        }
    }

    @Override
    public void print() {
        TrieNode currentTrieNode = root; //began at the root
        if (currentTrieNode.isWord) { //check if the node is a word
            System.out.println(currentTrieNode.word); //yes, print the word
        }
        if (!currentTrieNode.children.isEmpty()) {  //check if the children node has children
            Set<Character> nodes = currentTrieNode.children.keySet();

            nodes.forEach(node
                    -> {
                TrieNode nextNode = currentTrieNode.children.get(node); //get all the children nodes
                print(nextNode); //repeat for children nodes
            });
        }
    }

    public void print(TrieNode nextNode) {
        TrieNode currentTrieNode = nextNode; //began at the children node
        if (currentTrieNode.isWord) { //check if the children node is a word
            System.out.println(currentTrieNode.word); //yes, print the word
        }

        if (!currentTrieNode.children.isEmpty()) { //check if the children node has children
            Set<Character> nodes = currentTrieNode.children.keySet();

            nodes.forEach(node
                    -> {
                TrieNode nextNodeOfChildren = currentTrieNode.children.get(node); //get all the children nodes of children nodes
                print(nextNodeOfChildren); //repeat for children nodes of the children nodes
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
