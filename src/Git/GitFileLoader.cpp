#include <QTextStream>
#include <QtCore/QRegExp>

#include "Git/GitVersion.h"
#include "Git/GitFile.h"
#include "Git/GitFileDiffBlock.h"
#include "Git/GitFileDiffBlockLine.h"
QList<Git::GitVersion*> Git::GitFileLoader::getDataAboutGit() {

    QList<Git::GitVersion*> versions = QList<Git::GitVersion*>();;
    QString lTmp = makeTmpFileFromCommand( lGitCommand, lFilePath );
    if( ok  && file.open( QIODevice::ReadOnly ) ) {
        QTextStream reader( &file );
        QString line;        
        while( !reader.atEnd() ) {       
            line = reader.readLine();
            Git::GitVersion* version = new Git::GitVersion( line );

            line = reader.readLine();
            version->setAuthor( line );

            line = reader.readLine();
            version->setDate( line );

            bool read = true;
            while( read ) {
                line = reader.readLine();
                QRegExp A( "^A\\s" );
                QRegExp M( "^M\\s" );
                QRegExp D( "^D\\s" );

                if( A.indexIn( line ) != -1 ) {
                    line = line.replace(A, "projekt/");
                    Git::GitFile* file = new Git::GitFile( line.mid( line.lastIndexOf("/") + 1 ), line, Git::GitType::ADDED );
                    version->addChangedFile( file );
                } else if( M.indexIn( line ) != -1 ) {
                    line = line.replace(M, "projekt/");
                    Git::GitFile* file = new Git::GitFile( line.mid( line.lastIndexOf("/") + 1 ), line, Git::GitType::MODIFIED );
                    version->addChangedFile( file );
                } else if( D.indexIn( line ) != -1 ) {
                    line = line.replace(D, "projekt/");
                    Git::GitFile* file = new Git::GitFile( line.mid( line.lastIndexOf("/") + 1 ), line, Git::GitType::REMOVED );
                    version->addChangedFile( file );
                } else if( line.length() == 40 ) {
                    reader.readLine();
                    reader.readLine();
                } else {
                    read = false;
                }
            }
            versions.append( version );
        }
    }

    if( !file.remove() ) {
//    versions.at(9)->printVersion();

    return versions;
QString Git::GitFileLoader::makeTmpFileFromCommand( QString command, QString filepath ) {
    QString cwd = QDir::currentPath();
        QString lCommand = QString( command );
    if( !changeDir( cwd ) ) {
        qDebug() << "Nepodarilo sa vratit na povodny current working directory";
    }


Git::GitFile* Git::GitFileLoader::readGitShowFile( QString tmpFile, Git::GitFile* gitFile ) {
    bool ok = true;
    QFile lFile ( tmpFile );

    // kontrola na uspesnost ulozenia vystupu commandu do suboru
    if( !lFile.size() ) {
        ok = false;
    }
    if( ok && lFile.open( QIODevice::ReadOnly ) ) {
        QTextStream lReader( &lFile );
        QString lLine;
        int lLineNumber = 1;
        Git::GitFileDiffBlock* diffBlock = new Git::GitFileDiffBlock();
        while( !lReader.atEnd() ) {
            lLine = lReader.readLine();
            Git::GitFileDiffBlockLine* lDiffBlockLine = new Git::GitFileDiffBlockLine( lLine, lLineNumber++, Git::GitType::ADDED );
            diffBlock->incAddCount();
            diffBlock->addGitFileDiffBlockLine( lDiffBlockLine );
        }
        gitFile->addGitFileDiffBlock( diffBlock );
    }

    if( !lFile.remove() ) {
        qDebug() << "Nepodarilo sa odstranit temp subor";
    }

//    gitFile->getGitFileDiffBlocks().at(0)->printDiffBlockLines();

    return gitFile;
}

Git::GitFile* Git::GitFileLoader::readGitDiffFile( QString tmpFile, Git::GitFile* gitFile ) {
    bool ok = true;
    QFile lFile( tmpFile );

    // kontrola na uspesnost ulozenia vystupu commandu do suboru
    if( !lFile.size() ) {
        ok = false;
    }

    if( ok && lFile.open( QIODevice::ReadOnly ) ) {
        QTextStream lReader( &lFile );
        QString lLine;

        Git::GitFileDiffBlock* diffBlock = NULL;
        int iter = 4;


        // preskocime hlavicku diffu
        for(int i = 0; i < iter; i++) {
            lLine = lReader.readLine();
            if( lLine.indexOf("new file mode") >= 0 ) {
                iter++;
            }
            if( lLine.indexOf("deleted file mode") >= 0 ) {
                iter++;
            }
        }

        int addPos = 0, removePos = 0;

        while( !lReader.atEnd() ) {
            lLine = lReader.readLine();
            QRegExp regex;
            Git::GitFileDiffBlockLine* blockLine;
            switch ( lLine.at(0).toAscii() ) {
            case '@':
                if( diffBlock != NULL ) {
                    gitFile->addGitFileDiffBlock( diffBlock );
                }
                int firstComma, secondComma, middleSpace, endSpace;

                diffBlock = new Git::GitFileDiffBlock();

                // zistenie pozicii pre ciarky a medzery pre @@ -0,0 +0,0
                firstComma = lLine.indexOf(",", 4 );
                secondComma = lLine.indexOf(",", firstComma + 1 );
                middleSpace = lLine.indexOf(" ", firstComma );
                endSpace = lLine.indexOf(" ", middleSpace + 1 );

                diffBlock->setRemoveStart( lLine.mid( 4, firstComma - 4 ).toInt( false, 10 ) );
                diffBlock->setRemoveCount( lLine.mid( firstComma + 1, middleSpace - ( firstComma + 1 ) ).toInt( false, 10 ) );
                diffBlock->setAddStart( lLine.mid( middleSpace + 2, secondComma - ( middleSpace + 2 ) ).toInt( false, 10 ) );
                diffBlock->setAddCount( lLine.mid( secondComma + 1, endSpace - ( secondComma + 1 ) ).toInt( false, 10 ) );

                addPos = diffBlock->getAddStart();
                removePos = diffBlock->getRemoveStart();

                break;
            case '+':

                regex = QRegExp( "^\\+" );

                if( regex.indexIn( lLine ) != -1 ) {
                    lLine = lLine.replace( regex, "" );
                }

                blockLine = new Git::GitFileDiffBlockLine( lLine, addPos++ , Git::GitType::ADDED );
                diffBlock->addGitFileDiffBlockLine( blockLine );

                break;
            case '-':

                regex = QRegExp( "^-" );

                if( regex.indexIn( lLine ) != -1 ) {
                    lLine = lLine.replace( regex, "" );
                }

                blockLine = new Git::GitFileDiffBlockLine( lLine, removePos++ , Git::GitType::REMOVED );
                diffBlock->addGitFileDiffBlockLine( blockLine );

                break;
            case ' ':

                regex = QRegExp( "^ " );

                if( regex.indexIn( lLine ) != -1 ) {
                    lLine = lLine.replace( regex, "" );
                }

                removePos++;
                blockLine = new Git::GitFileDiffBlockLine( lLine, addPos++ , Git::GitType::NONE );
                diffBlock->addGitFileDiffBlockLine( blockLine );

                break;
            default:
                break;
            }
        }
        if( diffBlock != NULL ) {
            gitFile->addGitFileDiffBlock( diffBlock );
        }
    }

    if( !lFile.remove() ) {
        qDebug() << "Nepodarilo sa odstranit temp subor";
    }

//    gitFile->printContent();

    return gitFile;
}


Git::GitFile* Git::GitFileLoader::getDiffInfo( Git::GitFile* gitFile, QString currentCommitId, QString oldCommitId ) {
    bool ok = true;
    QString lCommand;
    QString lFilePath = this->filePath;

    // ak bol pridany, nemame predchadzajucu verziu suboru, preto nacitame celu verziu suboru
    if( gitFile->getType() == Git::GitType::ADDED ) {
        lCommand = QString( "git show " + currentCommitId + ":" + gitFile->getFilepath() );
    } else {
        lCommand = QString( "git diff -u " + oldCommitId + " " + currentCommitId + " -- " + gitFile->getFilepath() );
    }

    QString lTmp = makeTmpFileFromCommand( lCommand, lFilePath );

    if( ok  && gitFile->getType() == Git::GitType::ADDED ) {
        gitFile = readGitShowFile( lTmp, gitFile );
    } else {
        gitFile = readGitDiffFile( lTmp, gitFile );
    }

    return gitFile;
}
