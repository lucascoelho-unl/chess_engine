# Project planning

Hey Chat! I'm currently developing a project of a chess engine, and it is in progress. I'd like you to be my tutor for this project, and guide me trough steps for completing it. It is extremelly important that you don't give me code, just small snippets of function names if necessairy, but I want to do it on my own. The current project planning can be seen below: 

1. Board Representation
- Implement bitboard data structure
- Create functions for bitboard operations (set/clear/get bits)

2. Move Generation
- Implement piece movement rules using bitboards
- Generate legal moves for each piece type
- Handle special moves (castling, en passant, pawn promotion)

3. Game State
- Implement a way to represent the full game state (piece positions, whose turn, castling rights, en passant square, etc.)
- Create functions to make and unmake moves

4. Move Validation
- Implement check detection
- Ensure only legal moves are generated

5. Evaluation Function
- Develop a basic evaluation function (material count)
- Improve evaluation with positional factors

6. Search Algorithm
- Implement a basic Minimax algorithm
- Add Alpha-Beta pruning
- Implement iterative deepening

7. Opening Book
- Create a simple opening book structure
- Implement book move selection

8. Time Management
- Implement a basic time control system

9. UCI (Universal Chess Interface) Protocol
- Implement basic UCI commands to allow your engine to communicate with chess GUIs

10. Testing and Debugging
- Develop unit tests for each component
- Create a suite of test positions to verify move generation and evaluation
- Implement logging for debugging

11. Optimization
- Profile your code and optimize critical paths
- Implement more advanced search techniques (e.g., null move pruning, late move reductions)

12. Advanced Features (optional)
- Implement a transposition table
- Add endgame tablebases
- Develop machine learning-based evaluation

13. Web Interface Development
- Choose a web framework (e.g., React, Vue.js, or plain JavaScript)
- Design the chessboard UI
- Implement drag-and-drop functionality for moves
- Create a move history display
- Add game controls (start new game, undo move, etc.)

14. Engine Integration
- Create a WebAssembly (WASM) version of your C++ chess engine
- Implement a communication layer between the web interface and the WASM engine

15. Additional Features
- Add difficulty levels (adjust search depth or use different evaluation functions)
- Implement a PGN viewer/generator for game notation
- Create an analysis board for reviewing moves

16. Accessibility and Responsiveness
- Ensure the interface is responsive for different screen sizes
- Implement keyboard controls for accessibility

17. Performance Optimization
- Optimize the WASM build for size and speed
- Implement web workers for non-blocking engine calculations

18. Deployment
- Set up a GitHub repository for your project
- Create a build pipeline (e.g., using GitHub Actions)
- Deploy your website to a hosting service (e.g., GitHub Pages, Netlify, or Vercel)

19. Documentation
- Write a README explaining your project
- Document the architecture and key decisions
- Create a simple user guide for playing against your engine

20. Testing and Quality Assurance
- Implement end-to-end tests for the web interface
- Test on various browsers and devices
- Gather feedback from users and iterate on the design and functionality

# Current progress

This is all steps that I'll take towards the goal to finish making the entire chess engine. The project is in C++, and the frontend aspect will be decided further on. I'm currently on a mac device. And below, you can see my current progress: 

## Implemented bitboards representation and bitboards operation.
- I can set a bit
- I can clear a bit
- I can test a bit

## For the code to be more human-friendly, I implemented a Square enum, that gets various inputs (int, string, tuple) and transform them into chess notation that is equivalent to the square number of the bit. So A1 = 0, B1 = 1 ... H8 = 63.

## Implemented the board class that holds 12 bitboards, one for each piece type and color. Each bitboard represents the pieces as a whole in the chess game. 

## Implemented methods to get varous bitboards from the board class. 
- Get the bitboard from the pieces and colors
- Get a bitboard of all ocupied squares
- Get a bitboard of all empty squares
- Get a bitboard from all white or all black pieces
- Also a method to print the board for the human eye

## Implemented a method to read FEN notation into the board and create a new board with the pieces in the correct position based on the FEN notation. 

## Currently developing pieces movement bitboards. For each piece, I'll implement a function that returns a bitboard of all valid moves. The structure will be lioke below: 
// Get moves from a specific square for a piece of the specified color
Bitboard get_pawn_moves(Square from, Color color, const Board &board);
Bitboard get_knight_moves(Square from, Color color, const Board &board);
Bitboard get_bishop_moves(Square from, Color color, const Board &board);
Bitboard get_rook_moves(Square from, Color color, const Board &board);
Bitboard get_queen_moves(Square from, Color color, const Board &board);
Bitboard get_king_moves(Square from, Color color, const Board &board);
// Get all possible moves for the specified color
Bitboard get_all_pawn_moves(Color color, const Board &board);
Bitboard get_all_knight_moves(Color color, const Board &board);
Bitboard get_all_bishop_moves(Color color, const Board &board);
Bitboard get_all_rook_moves(Color color, const Board &board);
Bitboard get_all_queen_moves(Color color, const Board &board);
Bitboard get_all_king_moves(Color color, const Board &board);
// General function to get moves for any piece type
Bitboard get_piece_moves(Piece_Type piece, Square from, Color color, Bitboard occupancy);
// Generate all valid moves for the specified color
std::vector<Move> generate_all_valid_moves(Color color);

I'd love your help for instructions, next steps, how to proceed when I finish each step, and also with advices on how the project structure should be, and all that. 

I hope we have an amazing project together. 

Sincerely, 
Lucas. 