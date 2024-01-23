# Prefix Tree (Trie) Autocomplete
## Running the Program
There is no need to manually compile the program. It is executable via:
```
./autocomplete [k]
```
`k` is an optional parameter specifying **the number of autocompleted phrases you would like to see** during this instance of the program.

If you do not specify the parameter `k`, the default number of returned autocompleted phrases, **10**, will be used.

## Compilation
However, if you wish to compile the program yourself, run the make file

```
make
```

This program makes use of `filesystem`, which requires >= `G++ v8`. For example, Ubuntu `18.04` LTS will run into compilation errors where `filesystem` does not exists, as it has `G++ v7`.

Ensure you are using a version of GCC/G++ version >= 8 when compiling.

## Usage
Upon starting the program, the program reads and parses all `.txt` files in the `./corpus` folder to build the trie.

This will take some time, usually a few seconds. When the compilation is complete, the console prompts any text input:

![image](https://github.com/rui-han-crh/trie-autocomplete/assets/15359033/0913ef60-be0b-45a0-9a3e-53d45989cace)

After the `>`, you may input any string of characters. Pressing enter will allow the program to autocomplete your text. The first `k` (see [Running the Program](#running-the-program) for parameter description) nearest valid phrases will be autocompleted.

![image](https://github.com/rui-han-crh/trie-autocomplete/assets/15359033/2ca009a9-54c7-4c43-b36c-8115dee8b8b6)

Additionally, non-ASCII characters are also accepted:

![image](https://github.com/rui-han-crh/trie-autocomplete/assets/15359033/00254961-e8aa-4b74-974c-8e66c1a1d207)

## Expanding the Vocabulary
The knowledge base of the autocompletion program comes from all line separated words in each text file in the `./corpus` folder. To add your own words, simply modify an existing text file, or add additional files with words that are space separated.

Ensure these files end with the extensions `.txt`.

## Implementation

This project uses a trie structure to store a corpus of single, non-space separated, words. These words may not be ASCII, and may be contain multi-byte characters (Chinese or Japanese characters, for example).

A trie is a tree like data structure, containing a set of nodes. All nodes have a set of children and a stored data. In addition, each node, apart from the roots, has a parent. In this project, there are 256 child nodes per parent node and each node stores a byte.

Files are specifies in the `corpus` directory, containing `.txt` files with line separated words (not necessarily in English). During application start-up, the program read every `.txt` file in the corpus directory and parses every string of words line-by-line.

For each (potentially multibyte-character) string, it is transformed into UTF-8 to maintain byte consistency.

The trie stores byte sequences. A transition from any parent node to a child node indicts immediate succession of the parent node's byte with the child node's byte.

For example, consider the byte sequences for `banana`, `balloon` and `ballot`:
| English Word | Byte Sequence |
|-|-|
| banana | 0x62 0x61 0x6e 0x61 0x6e 0x61 |
| balloon | 0x62 0x61 0x6c 0x6c 0x6f 0x6f 0x6e |
| ballot | 0x62 0x61 0x6c 0x6c 0x6f 0x74 |

Observe the similarity in some ordered positions of some byte sequences. We are able to represent these three words in as a trie:
![image](https://github.com/rui-han-crh/trie-autocomplete/assets/15359033/104a46ef-f866-4b63-b554-6c08477b6650)

Similarly, a new word may extend the trie from any byte as along as it has the same prefix from the parent that it is a child of to the root of the tree.

A trie has 256 roots, one for each starting byte beginning the sequence. Each of the 256 roots, as well as any 256 children of any node, it represented by a 256 node pointer array. If a byte character `b_j` succeeds a current byte character `b_i`, then that pointer in the children array will point to a valid Node object, specifically at the position that is the _unsigned char_ representation of `b_j`. Otherwise, it is a `nullpointer` indictating the corpus contains no word that has a byte sequence where `b_j` succeeds `b_i`.

To mark that a sequence of bytes is completed, the _null terminator_ is used. This is stored in position 0 and is represented by `0x0`.

In the above example, suppose the word `Ball` exists as well. With null terminators, the trie is able to represent exactly where valid sequences end.

![image](https://github.com/rui-han-crh/trie-autocomplete/assets/15359033/cc5c7f4c-67b4-41cb-9806-3b17e137c8c4)


### Rational for Choosing Byte Sequence over Character Sequence
ASCII tries can be represented with character sequences, where keys are likely to be alphanumeric with special characters. However, extended corpuses may include characters outside of the standard ASCII range, such as Chinese characters and other Unicode characters. These characters are considered multibyte characters extending beyond 1 byte long. 

In Chinese alone, there are 40,000+ different characters, potentially more if other languages are included. Such becomes infeasible for fixed-sized children arrays.

One workaround is a hashing solution, but I wanted to avoid complicated algorithms to maintain O(1) access.

The alternative was to interpret every string, regardless of its contents, as the smallest unit of data. Hence, the program uses bytes (`char`) as data stores per node.
