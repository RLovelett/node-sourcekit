declare module SourceKit {

    export interface ICursorOption {
        sourcefile: string,
        offset: number,
        compilerargs: string[]
    }

    export interface ICursorInfo {
        'key.kind': string,
        'key.name': string,
        'key.usr': string,
        'key.filepath': string,
        'key.offset': number,
        'key.length': number,
        'key.typename': string
        'key.annotated_decl': string
        'key.fully_annotated_decl': string
        'key.typeusr': string
    }

    export function cursorinfo(options: ICursorOption): Promise<ICursorInfo>

}

export = SourceKit;
