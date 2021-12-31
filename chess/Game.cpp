#include "Game.h"

Game::Game() {
	graphics = Graphics::Instance();
	audioManager = AudioManager::Instance();

	white = Piece::COLOR::white;
	black = Piece::COLOR::black;

	// all pawns
	for (int y = 1; y <= 8; y++) {
		for (int x = 1; x <= 8; x++) {
			switch (y) {
			case 1:
				switch (x) {
				case 1:
				case 8: {
					grid[{x, y}] = new Piece(x, y, black, Piece::TYPE::rook);
					break;
				}
				case 2:
				case 7:
					grid[{x, y}] = new Piece(x, y, black, Piece::TYPE::knight);
					break;
				case 3:
				case 6:
					grid[{x, y}] = new Piece(x, y, black, Piece::TYPE::bishop);
					break;
				case 4:
					grid[{x, y}] = new Piece(x, y, black, Piece::TYPE::king);
					break;
				case 5:
					grid[{x, y}] = new Piece(x, y, black, Piece::TYPE::queen);
					break;
				}
				break;
			case 2:
				grid[{x, y}] = new Piece(x, y, black, Piece::TYPE::pawn);
				break;
			case 7:
				grid[{x, y}] = new Piece(x, y, white, Piece::TYPE::pawn);
				break;
			case 8:
				switch (x) {
				case 1:
				case 8:
					grid[{x, y}] = new Piece(x, y, white, Piece::TYPE::rook);
					break;
				case 2:
				case 7:
					grid[{x, y}] = new Piece(x, y, white, Piece::TYPE::knight);
					break;
				case 3:
				case 6:
					grid[{x, y}] = new Piece(x, y, white, Piece::TYPE::bishop);
					break;
				case 4:
					grid[{x, y}] = new Piece(x, y, white, Piece::TYPE::king);
					break;
				case 5:
					grid[{x, y}] = new Piece(x, y, white, Piece::TYPE::queen);
					break;
				}
				break;
			default:
				grid[{x, y}] = nullptr;
				break;
			}
		}
	}

	turn = white;
	selected = nullptr;
	holdingLMB = false;
	check = false;
	quit = false;
}

void Game::PrintBoard() {
	for (int y = 1; y <= 8; y++) {
		for (int x = 1; x <= 8; x++) {
			if (grid[{x, y}]) {
				printf(" %c ", grid[{x, y}]->color == white ? 'w' : 'b');
			}
			else {
				printf(" - ");
			}
		}
		printf("\n");
	}
	printf("\n");
	printf("-----------------------\n\n");
}

void Game::DrawBoard() {
	for (int y = 0; y < 8; y++) {
		if (y % 2 == 0) {
			for (int x = 0; x < 8; x++) {
				SDL_Rect square = { 128 * x, 128 * y, 128, 128 };
				if (x % 2 == 0) {
					graphics->DrawRect(square, 100, 50, 0);
				}
				else
					graphics->DrawRect(square, 255, 255, 255);
			}
		}
		else {
			for (int x = 8; x > -1; x--) {
				SDL_Rect square = { 128 * x, 128 * y, 128, 128 };
				if (x % 2 == 0) {
					graphics->DrawRect(square, 255, 255, 255);
				}
				else
					graphics->DrawRect(square, 100, 50, 0);
			}
		}
	}
}

bool Game::Check() {

	std::vector<Piece*> kings;
	std::vector<Vector> kingsPos;
	int count = 0;

	// Get kings
	for (auto& square : grid) {
		if (square.second && square.second->type == Piece::TYPE::king) {
			kingsPos.push_back(square.first);
			kings.push_back(square.second);
			count++;
		}

		if (count > 1) {
			break;
		}
	}

	int i = 0;
	for (auto& king : kings) {
		for (auto& square : grid) {
			if (square.second && square.first != kingsPos[i] && square.second->color != king->color) {
				SDL_Point point = { kingsPos[i].x, kingsPos[i].y };
				square.second->Drag(point, true);

				// if valid move, there is a check
				if (square.second->ValidMove(grid)) {
					printf("Check!\n\n");
					return true;
				}
			}
		}
		i++;
	}

	return false;
}

bool Game::CheckMate() {

	bool checkMate = true;

	for (auto& square : grid) {
		if (square.second && square.second->color == turn) {
			for (int x = 1; x <= 8; x++) {
				for (int y = 1; y <= 8; y++) {
					Piece* selected = square.second;
					SDL_Point point = { x, y };
					selected->Drag(point, true);

					if (selected->ValidMove(grid)) {
						Vector prevPos = selected->GetPosition();
						Vector nextPos = selected->GetDraggedPosition();
						Piece* savePiece = nullptr;

						if (grid[nextPos]) {
							savePiece = grid[nextPos];
						}

						grid[prevPos] = nullptr;
						grid[nextPos] = selected;

						if (!Check()) {
							checkMate = false;
							grid[prevPos] = selected;
							grid[nextPos] = savePiece;
							break;
						}

						grid[prevPos] = selected;
						grid[nextPos] = savePiece;
					}
				}
			}
		}
	}

	if (checkMate) {
		printf("Checkmate!\n");
	}

	return false;
}

void Game::Run() {
	double firstFrame;
	double lastFrame = 0;

	PrintBoard();

	while (!quit) {
		
		EarlyUpdate();
		Update();
		Render();
		
		if (SDL_WaitEvent(&event)) {
			if (event.type == SDL_QUIT) {
				quit = true;
			}
		}

		/*firstFrame = SDL_GetTicks();
		if (firstFrame - lastFrame < (1000 / 60)) {
			SDL_Delay((1000 / 60) - (firstFrame - lastFrame));
		}
		lastFrame = firstFrame;*/
	}
}

void Game::EarlyUpdate() {
	switch (event.type) {
	case SDL_MOUSEMOTION:
		mousePos = { event.motion.x, event.motion.y };
		if (holdingLMB && selected != nullptr) {
			selected->Drag(mousePos);
		}
		break;
	case SDL_MOUSEBUTTONDOWN:
		if (!holdingLMB && event.button.button == SDL_BUTTON_LEFT) {
			holdingLMB = true;

			for (auto& pawn : grid) {
				if (pawn.second) {
					if (SDL_PointInRect(&mousePos, pawn.second->getPiece()) && pawn.second->color == turn) {
						selected = pawn.second;
						break;
					}
				}
			}
		}
		break;
	case SDL_MOUSEBUTTONUP:
		if (holdingLMB && event.button.button == SDL_BUTTON_LEFT) {
			holdingLMB = false;

			if (selected) {
				if (selected->ValidMove(grid)) {
					Vector prevPos = selected->GetPosition();
					Vector futurePos = selected->GetDraggedPosition();

					Piece* savePiece = nullptr;
					if (grid[futurePos]) {
						savePiece = grid[futurePos];
					}

					grid[prevPos] = nullptr;
					grid[futurePos] = selected;

					if (Check()) {
						selected->ReturnToPosition();
						grid[prevPos] = selected;
						grid[futurePos] = savePiece;
					}
					else {
						selected->PositionOnBoard(futurePos);
						if (savePiece) {
							delete savePiece;
						}
						turn = turn == white ? black : white;
						if (Check()) {
							CheckMate();
						}
						PrintBoard();
						audioManager->PlaySFX("move.wav");
					}
				}
				else {
					selected->ReturnToPosition();
				}
			}
			selected = nullptr;
		}
		break;
	}
}

void Game::Update() {
	for (auto& pawn : grid) {
		if (pawn.second) {
			pawn.second->Update();
		}
	}
}

void Game::Render() {

	graphics->Clear();

	// RENDER GAME OBJECTS HERE //

	DrawBoard();

	for (auto& pawn : grid) {
		if (pawn.second) {
			pawn.second->Render();
		}		
	}
	
	/////////////////////////////

	graphics->Render();
}

void Game::Quit() {
	graphics->Quit();
}