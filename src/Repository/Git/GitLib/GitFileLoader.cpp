Repository::Git::GitFileLoader::GitFileLoader()
Repository::Git::GitFileLoader::GitFileLoader( QString filepath, QString extensions ) : filePath( filepath ), extensions( extensions )
QList<Repository::Git::GitVersion*> Repository::Git::GitFileLoader::getDataAboutGit()
    QList<Repository::Git::GitVersion*> versions = QList<Repository::Git::GitVersion*>();;
            Repository::Git::GitVersion* version = new Repository::Git::GitVersion( line );
                    Repository::Git::GitFile* file = new Repository::Git::GitFile( line.mid( line.lastIndexOf( "/" ) + 1 ), line, Repository::Git::GitType::ADDED );
                    Repository::Git::GitFile* file = new Repository::Git::GitFile( line.mid( line.lastIndexOf( "/" ) + 1 ), line, Repository::Git::GitType::MODIFIED );
                    Repository::Git::GitFile* file = new Repository::Git::GitFile( line.mid( line.lastIndexOf( "/" ) + 1 ), line, Repository::Git::GitType::REMOVED );
QString Repository::Git::GitFileLoader::makeTmpFileFromCommand( QString command, QString filepath )
bool Repository::Git::GitFileLoader::changeDir( QString path )
bool Repository::Git::GitFileLoader::existGit( QString path )
void Repository::Git::GitFileLoader::readGitShowFile( QString tmpFile, Repository::Git::GitFile* gitFile )
        Repository::Git::GitFileDiffBlock* diffBlock = new Repository::Git::GitFileDiffBlock();
            Repository::Git::GitFileDiffBlockLine* lDiffBlockLine = new Repository::Git::GitFileDiffBlockLine( lLine, lLineNumber++, Repository::Git::GitType::ADDED );
void Repository::Git::GitFileLoader::readGitDiffFile( QString tmpFile, Repository::Git::GitFile* gitFile )
        Repository::Git::GitFileDiffBlock* diffBlock = NULL;
            Repository::Git::GitFileDiffBlockLine* blockLine;
                    diffBlock = new Repository::Git::GitFileDiffBlock();
                    blockLine = new Repository::Git::GitFileDiffBlockLine( lLine, addPos++ , Repository::Git::GitType::ADDED );
                    blockLine = new Repository::Git::GitFileDiffBlockLine( lLine, removePos++ , Repository::Git::GitType::REMOVED );
                    blockLine = new Repository::Git::GitFileDiffBlockLine( lLine, addPos++ , Repository::Git::GitType::NONE );
void Repository::Git::GitFileLoader::getDiffInfo( Repository::Git::GitFile* gitFile, QString currentCommitId, QString oldCommitId )
    if ( gitFile->getType() == Repository::Git::GitType::ADDED ) {
    if ( gitFile->getType() == Repository::Git::GitType::ADDED ) {