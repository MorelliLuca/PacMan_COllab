// Fill out your copyright notice in the Description page of Project Settings.


#include "GridPawn.h"
#include "GridPlayerController.h"
#include "TestGridGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGridPawn::AGridPawn()
{
	//
	// N.B il pawn viene spawnato automaticamente nella posizione del player start
	// dato che il pawn di default è stato impostato nei setting come BP_GridPawn
	//  
	// MY ADD: PacManSpawnNode (9,13) -> (950, 1350, 5)
	// 
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//// Set this pawn to be controlled by the lowest-numbered player
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	//// vettore di direzione inizializzato con zero (il pawn non si muove allo start del gioco fino a che non
	//   viene premuto uno dei tasti W-A-S-D )
	LastInputDirection = FVector(0, 0, 0);
	LastValidInputDirection = FVector(0, 0, 0);
	////posizione iniziale  del pawn nelle coordinate di griglia (1,1)  -> OLD
	////posizione iniziale  del pawn nelle coordinate di griglia (9,13) -> NEW
	CurrentGridCoords = FVector2D(9, 13);
	//// nodi
	LastNode = nullptr;
	TargetNode = nullptr;
	NextNode = nullptr;

}

// Called when the game starts or when spawned
void AGridPawn::BeginPlay()
{
	Super::BeginPlay();
	GameMode = (ATestGridGameMode*)(GetWorld()->GetAuthGameMode());
	TheGridGen = GameMode->GField;
	//// posizione iniziale del pawn (è quella del PlayerStart)
	FVector2D StartNode = TheGridGen->GetXYPositionByRelativeLocation(GetActorLocation());
	LastNode = TheGridGen->TileMap[StartNode];

	//MY ADD così ho il puntatore a TMap delle tile
	GridGenTMap = TheGridGen->GetTileMAp();
}

// Called every frame
void AGridPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AGridPawn::HandleMovement();
}

void AGridPawn::HandleMovement()
{
	//SE SONO IN TELEPORT NODE
	MoveToCurrentTargetNode();
	if (TargetNode == nullptr)
	{
		if (NextNode != nullptr)
		{
			SetTargetNode(NextNode);
			SetNextNode(nullptr);
		}
	}
	// questo codice fa si che il pawn si muova autonomamente fino a che il next node è walkable
	if (!TargetNode && !NextNode)
	{
		if (TheGridGen->IsNodeValidForWalk(TheGridGen->GetNextNode(CurrentGridCoords, LastInputDirection)))
		{
			SetLastValidDirection(LastInputDirection);
		}
		SetNodeGeneric(LastValidInputDirection);
	}
}

void AGridPawn::MoveToCurrentTargetNode()
{
	if (TargetNode == nullptr) return;
	
	//SE NON LA USO NO PROBLEM XKE' E' UNA VARIBILE INTERNA CHE NON MODIFICA I VALORI DEI PUNTATORI
	//ED SE FOSSE NULL CMQ NON AVVEREBBE IL TRASPORTO XKE' NELL TELEPORT IF AVREI CONDIZIONE FALSA

	/*FVector2D last_node_2Dpos = FVector2D(0, 0);
	if (LastNode != nullptr)
	{
		last_node_2Dpos = LastNode->GetGridPosition();
	}*/


	//MY ADD TELEPORT DA (18,27) -> (18,0)
	//check che non sono in TeleportNode posizioni (18,0) con versore (0,-1,0) ed (18,27) con versore (0, 1, 0)
	//DA DEBUG SI VEDE CHE TIENE LA POSIZIONE PRIMA 
	//(es: dir=(0,1,0) DESTRA, e sono in realtà in (18,27) e current gridpos=(18,26))
	
	//TELEPORT IF
	//if (CurrentGridCoords == FVector2D(18, 27) && LastValidInputDirection == FVector(0, 1, 0)){	// && LastValidInputDirection == FVector(0, 1, 0)
	//if (last_node_2Dpos == FVector2D(18, 27) && LastValidInputDirection == FVector(0, 1, 0)){	// && LastValidInputDirection == FVector(0, 1, 0)
		//const FVector Location(1850.0f, 0.0f, GetActorLocation().Z);
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("POSIZIONE (18,27) RAGIUNTA")));

		////IN TEORIA DIR RIMANE VALIDA COSI' E BON
		////Set current grid position
		//CurrentGridCoords = FVector2D(18, 0);
		////set lastnode
		//LastNode = *(GridGenTMap.Find(FVector2D(18, 0)));
		////set nextnode
		//SetNextNode(*(GridGenTMap.Find(FVector2D(18, 1))));
		////set targetnode
		//SetTargetNode(NextNode);

		////OLD
		///*NextNode = *(GridGenTMap.Find(FVector2D(18, 1)));*/

		////teleport pacman to (18,0)
		//SetActorLocation(Location);
	/*}
	else
	{*/
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("AVANTI")));
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Position x=%f  Y=%f "), CurrentGridCoords.X, CurrentGridCoords.Y));

		//-----------------------------------------------------------------------------------------
		//MY ADD
		//CHECK PRINTF
		//print lastnode pos
		// if(LastNode == nullptr)
		// {
		//	 GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Last node: NULLPTR")));
		// }
		// else{
		//	const FVector2D last_node_2Dpos = LastNode->GetGridPosition();
		//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Last node: (%f, %f)"), last_node_2Dpos.X, last_node_2Dpos.Y));
		// }

		////print nextnode pos
		////NON LO USA MAI 
		// if (NextNode == nullptr)
		// {
		//	 GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Next node: NULLPTR")));
		// }
		// else {
		//	 const FVector2D next_node_2Dpos = NextNode->GetGridPosition();
		//	 GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Next node: (%f, %f)"), next_node_2Dpos.X, next_node_2Dpos.Y));
		// }

		////print targetnode pos
		// if (TargetNode == nullptr)
		// {
		//	 GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Target node: NULLPTR")));
		// }
		// else {
		//	 const FVector2D target_node_2Dpos = TargetNode->GetGridPosition();
		//	 GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Target node: (%f, %f)"), target_node_2Dpos.X, target_node_2Dpos.Y));
		// }
		//--------------------------------------------------------------------------------------

		const float Dist = FMath::Abs(FVector::Dist2D(TargetNode->GetActorLocation(), GetActorLocation()));
		if (Dist <= AcceptedDistance)
		{
			OnNodeReached();
			return;
		}
		// funzione di interpolazione che fa muovere il pawn verso una nuova posizione data la posizione corrente
		const FVector2D StartVector = TheGridGen->GetTwoDOfVector(GetActorLocation());
		const FVector2D EndVector = TheGridGen->GetTwoDOfVector(TargetNode->GetActorLocation());
		const auto Pos = FMath::Vector2DInterpConstantTo(StartVector, EndVector, GetWorld()->GetDeltaSeconds(), CurrentMovementSpeed);
		const FVector Location(Pos.X, Pos.Y, GetActorLocation().Z);
		SetActorLocation(Location);
	//}
}

void AGridPawn::OnNodeReached()
{
	CurrentGridCoords = TargetNode->GetGridPosition();
	LastNode = TargetNode;
	SetTargetNode(nullptr);
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("ONREACHED")));

	//NEW
	/*CurrentGridCoords = TargetNode->GetGridPosition() + TheGridGen->GetTwoDOfVector(LastValidInputDirection);
	LastNode = TargetNode;
	SetTargetNode(nullptr);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("MI FERMOOOO")));*/

	//print lastnode pos
	//if (LastNode == nullptr)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Last node: NULLPTR")));
	//}
	//else {
	//	const FVector2D last_node_2Dpos = LastNode->GetGridPosition();
	//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Last node: (%f, %f)"), last_node_2Dpos.X, last_node_2Dpos.Y));
	//}

	////print nextnode pos
	////NON LO USA MAI 
	//if (NextNode == nullptr)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Next node: NULLPTR")));
	//}
	//else {
	//	const FVector2D next_node_2Dpos = NextNode->GetGridPosition();
	//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Next node: (%f, %f)"), next_node_2Dpos.X, next_node_2Dpos.Y));
	//}

	////print targetnode pos
	//if (TargetNode == nullptr)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Target node: NULLPTR")));
	//}
	//else {
	//	const FVector2D target_node_2Dpos = TargetNode->GetGridPosition();
	//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Target node: (%f, %f)"), target_node_2Dpos.X, target_node_2Dpos.Y));
	//}
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("MI FERMO")));

	// check che non sono in TeleportNode posizioni (18,0) con versore (0,-1,0)
	// ed (18,27) con versore (0, 1, 0)
	const FVector2D last_node_2Dpos = LastNode->GetGridPosition();

	//TELEPORT IF (18,27) -> (18,0)
	if (last_node_2Dpos == FVector2D(18, 27) && LastValidInputDirection == FVector(0, 1, 0))
	{
		const FVector Location(1850.0f, 50.0f, GetActorLocation().Z);
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("POSIZIONE (18,27) RAGIUNTA")));

		//IN TEORIA DIR RIMANE VALIDA COSI' E BON
		//Set current grid position
		CurrentGridCoords = FVector2D(18, 0);
		//set lastnode
		LastNode = *(GridGenTMap.Find(FVector2D(18, 0)));
		//set nextnode QUELLO A DESTRA DI DOVE TELEPORT
		SetNextNode(*(GridGenTMap.Find(FVector2D(18, 1))));
		//set targetnode
		SetTargetNode(NextNode);

		//teleport pacman to (18,0)
		SetActorLocation(Location);
	}

	//TELEPORT IF (18,0) -> (18,27)
	if (last_node_2Dpos == FVector2D(18, 0) && LastValidInputDirection == FVector(0, -1, 0))
	{
		const FVector Location(1850.0f, 2750.0f, GetActorLocation().Z);
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("POSIZIONE (18,0) RAGIUNTA")));

		//IN TEORIA DIR RIMANE VALIDA COSI' E BON
		//Set current grid position
		CurrentGridCoords = FVector2D(18, 27);
		//set lastnode
		LastNode = *(GridGenTMap.Find(FVector2D(18, 27)));
		//set nextnode QUELLO A SINISTRA DI DOVE TELEPORT
		SetNextNode(*(GridGenTMap.Find(FVector2D(18, 26))));
		//set targetnode
		SetTargetNode(NextNode);

		//teleport pacman to (18,0)
		SetActorLocation(Location);
	}
}

void AGridPawn::SetTargetNode(AGridBaseNode* Node)
{
	TargetNode = Node;
}

void AGridPawn::SetNextNode(AGridBaseNode* Node)
{
	NextNode = Node;
}

void AGridPawn::SetNodeGeneric(const FVector Dir)
{
	const auto Node = TheGridGen->GetNextNode(CurrentGridCoords, Dir);
	if (TheGridGen->IsNodeValidForWalk(Node))
	{
		SetTargetNode(Node);
	}
}

void AGridPawn::OnClick()
{
	FHitResult Hit = FHitResult(ForceInit);
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, true, Hit);
	if (AGridBaseNode* CurrTile = Cast<AGridBaseNode>(Hit.GetActor()))
	{
		FVector2D CurrCoords = CurrTile->GetGridPosition();
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Position x=%f  Y=%f "), CurrCoords.X, CurrCoords.Y));
	}

}

void AGridPawn::SetVerticalInput(float AxisValue)
{
	if (AxisValue == 0) return;
	const FVector Dir = (GetActorForwardVector() * AxisValue).GetSafeNormal();
	LastInputDirection = Dir.GetSafeNormal();
	SetNextNodeByDir(LastInputDirection);

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("SetVerticalInput")));
}

void AGridPawn::SetHorizontalInput(float AxisValue)
{
	if (AxisValue == 0) return;
	const FVector Dir = (GetActorRightVector() * AxisValue).GetSafeNormal();
	LastInputDirection = Dir;
	SetNextNodeByDir(LastInputDirection);

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("SetHorizontalInput")));
}

void AGridPawn::SetNextNodeByDir(FVector InputDir)
{
	//conditional operator  (condition)? expr_if_true : expr_if_false 
	const FVector2D Coords = TargetNode ? TargetNode->GetGridPosition() : LastNode->GetGridPosition();
	const auto Node = GameMode->GField->GetNextNode(Coords, InputDir);
	if (GameMode->GField->IsNodeValidForWalk(Node))
	{
		SetNextNode(Node);
		SetLastValidDirection(InputDir);
	}
}

FVector AGridPawn::GetLastValidDirection() const
{
	return LastValidInputDirection;
}

void AGridPawn::SetLastValidDirection(FVector Dir)
{
	if (Dir == FVector::ZeroVector) return;
	LastValidInputDirection = Dir;
}